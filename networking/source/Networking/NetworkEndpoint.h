#pragma once

#include <Common/Common.h>
#include "Networking.h"
#include "Socket/Base/BaseSocket.h"
#include "Packets/PacketHeader.h"
#include "Packets/PendingPacket.h"
#include "WinsockContext/WinsockContext.h"
#include "PacketSendStrategy.h"
#include "PendingPacketsQueueManager/PendingPacketsQueueManager.h"
#include <vector>
#include <type_traits>

namespace networking
{
	template <class TSocket>
	class NetworkEndpoint : public common::NonCopyableObject
	{
		static_assert(std::is_convertible_v<TSocket *, BaseSocket *>,
					  "TSocket must publicly inherit from BaseSocket");

	protected:
		NetworkEndpoint(IN const std::string &strIpAddress, IN const std::string &strPort)
			: m_strIpAddress(strIpAddress), m_strPort(strPort)
		{
		}

	public:
		virtual ~NetworkEndpoint()
		{
		}

	public:
		virtual bool Start() PURE;
		virtual bool IsPacketAlreadyQueued(IN const ReceivedPacket &oReceivedPacket) const PURE;
		virtual bool ProcessPacket(IN const ReceivedPacket &oReceivedPacket) PURE;

	public:
		template <class TPacket>
		bool SendPacket(IN TPacket &oPacket, IN const SocketAddress *pSocketAddress = nullptr, IN const PacketSendStrategy ePacketSendStrategy = PacketSendStrategy::Unreliable)
		{
			static_assert(std::is_convertible_v<TPacket *, Packet *>,
						  "TPacket must publicly inherit from Packet");

			Packet *const pBasePacket = static_cast<Packet *>(&oPacket);
			if (!pBasePacket)
			{
				// TODO ERROR
				return false;
			}

			if (pBasePacket->nnRequestId <= 0)
				pBasePacket->nnRequestId = m_nnNextRequestId++;

			const char *pPacketData = SerializePacket(oPacket);
			if (!pPacketData)
			{
				LOG_ERROR("An error occured while serializing packet: {0}", GetPacketName(pBasePacket->ePacketType));
				return false;
			}

			if (ePacketSendStrategy == PacketSendStrategy::Reliable)
			{
				PendingPacket oPendingPacket;
				oPendingPacket.oPacketHeader = *pBasePacket;
				oPendingPacket.oPacketSerializedData = std::vector<char>(pPacketData, pPacketData + sizeof(TPacket));

				m_oPendingPacketsQueueManager.Submit(oPendingPacket);
			}

			if (!SendPacketInternal(pPacketData, sizeof(TPacket), pSocketAddress))
			{
				// TODO LOG
				return false;
			}

			LOG_DEBUG(
				"Packet sent | Type: {0} | RequestId: {1} | Size: {2} bytes",
				GetPacketName(pBasePacket->ePacketType),
				pBasePacket->nnRequestId,
				sizeof(TPacket));

			return true;
		}

	protected:
		void RecievePackets()
		{
			while (true)
			{
				char messageBuffer[n_MAX_PACKET_SIZE]{};
				SocketAddress oClientAddress;

				const auto nBytesRecived = m_oSocket->Recieve(messageBuffer, &oClientAddress);
				if (nBytesRecived == SOCKET_ERROR)
				{
					auto nNetworkError = WSAGetLastError();

					if (nNetworkError == WSAEWOULDBLOCK)
						break;

					return;
				}

				const PacketHeader *const pPacketHeader = reinterpret_cast<PacketHeader *>(messageBuffer);

				if (!pPacketHeader)
					continue;

				if (!ValidatePacketHeader(*pPacketHeader, nBytesRecived))
				{
					LOG_ERROR("The recieved packet is corrupted: {0}", GetPacketName(pPacketHeader->ePacketType));
					continue;
				}

				LOG_DEBUG(
					"Received packet | Type: {0} | RequestId: {1} | Size: {2} bytes | From: {3}",
					GetPacketName(pPacketHeader->ePacketType),
					pPacketHeader->nnRequestId,
					nBytesRecived,
					oClientAddress.GetIpAddress());

				RecievedPacket oRecievedPacket;
				oRecievedPacket.nnRequestId = pPacketHeader->nnRequestId;
				oRecievedPacket.ePacketType = pPacketHeader->ePacketType;
				oRecievedPacket.oSocketAddress = oClientAddress;
				oRecievedPacket.nPacketSize = nBytesRecived;

				if (!IsPacketAlreadyQueued(oRecievedPacket))
				{
					LOG_WARN(
						"Duplicate packet ignored | Type: {0} | RequestId: {1} | From: {2}",
						GetPacketName(oRecievedPacket.ePacketType),
						oRecievedPacket.nnRequestId,
						oRecievedPacket.oSocketAddress.GetIpAddress());
					continue;
				}

				memcpy(oRecievedPacket.oPacketPayload, messageBuffer, nBytesRecived);
				memset(messageBuffer, 0, nBytesRecived);

				const auto nnPendingPacketId = m_oPendingPacketsQueueManager.FindPendingPacket(oRecievedPacket);

				if (nnPendingPacketId != ELEMENT_NOT_FOUND)
				{
					m_oPendingPacketsQueueManager.Remove(nnPendingPacketId);
				}

				m_oRecivedPacketsQueue.push_back(oRecievedPacket);
			}
		}

		void ProcessPackets()
		{
			for (auto packetIterator = m_oRecivedPacketsQueue.begin(); packetIterator != m_oRecivedPacketsQueue.end();)
			{
				const auto ePacketType = packetIterator->ePacketType;
				if (!ProcessPacket(*packetIterator))
				{
					++packetIterator;
					continue;
				}

				packetIterator = m_oRecivedPacketsQueue.erase(packetIterator);
			}
		}

		void ProcessPendingPackets()
		{
			const std::vector<PendingPacket> &oPendingPackets = m_oPendingPacketsQueueManager.GetPacketsToRetry();
			for (const auto &oPendingPacket : oPendingPackets)
			{
				const char *pPacketData = oPendingPacket.oPacketSerializedData.data();

				if (!SendPacketInternal(pPacketData, oPendingPacket.oPacketSerializedData.size()))
				{
					// TODO LOG
				}
			}
		}

	private:
		bool ValidatePacketHeader(IN const PacketHeader &packetHeader, IN const int nBytesRecived)
		{
			const auto nPacketSize = sizeof(PacketHeader);

			if (nBytesRecived < nPacketSize)
			{
				LOG_ERROR("Incomplete packet recieved | Expected: {0} | Actual: {1}",
						  nPacketSize, nBytesRecived);

				return false;
			}

			return true;
		}

		bool SendPacketInternal(IN const char *pPacketData, IN const int nPacketSize, IN const SocketAddress *pSocketAddress = nullptr)
		{
			const auto nBytesSent = m_oSocket->Send(pPacketData, nPacketSize, pSocketAddress);

			if (nBytesSent == SOCKET_ERROR)
			{
				LOG_ERROR("Failed to send packet");
				return false;
			}

			return true;
		}

	protected:
		const std::string m_strIpAddress;
		const std::string m_strPort;

		common::Scope<networking::WinsockContext> m_oWinsockContext = nullptr;
		common::Scope<TSocket> m_oSocket = nullptr;

		std::vector<networking::RecievedPacket> m_oRecivedPacketsQueue;

		PendingPacketsQueueManager &m_oPendingPacketsQueueManager = PendingPacketsQueueManager::GetInstance();

	private:
		__int64 m_nnNextRequestId = 1;
	};
}