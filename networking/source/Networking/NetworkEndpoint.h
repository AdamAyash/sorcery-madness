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
#include <optional>

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
		bool SendPacket(IN TPacket &oPacket, IN const SocketAddress *pSocketAddress = nullptr,
						IN const PacketSendStrategy ePacketSendStrategy = PacketSendStrategy::Unreliable)
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
				LOG_ERROR("An error occurred while serializing packet: {0}", GetPacketName(pBasePacket->ePacketType));
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
		void ReceivePackets()
		{
			while (true)
			{
				char messageBuffer[n_MAX_PACKET_SIZE]{};
				SocketAddress oClientAddress;

				const auto nBytesReceived = m_oSocket->Receive(messageBuffer, &oClientAddress);
				if (nBytesReceived == SOCKET_ERROR)
				{
					auto nNetworkError = WSAGetLastError();

					if (nNetworkError == WSAEWOULDBLOCK)
						break;

					return;
				}

				const PacketHeader *const pPacketHeader = reinterpret_cast<PacketHeader *>(messageBuffer);

				if (!pPacketHeader)
					continue;

				if (!ValidatePacketHeader(*pPacketHeader, nBytesReceived))
				{
					LOG_ERROR("The received packet is corrupted: {0}", GetPacketName(pPacketHeader->ePacketType));
					continue;
				}

				LOG_DEBUG(
					"Received packet | Type: {0} | RequestId: {1} | Size: {2} bytes | From: {3}",
					GetPacketName(pPacketHeader->ePacketType),
					pPacketHeader->nnRequestId,
					nBytesReceived,
					oClientAddress.GetIpAddress());

				ReceivedPacket oReceivedPacket;
				oReceivedPacket.nnRequestId = pPacketHeader->nnRequestId;
				oReceivedPacket.ePacketType = pPacketHeader->ePacketType;
				oReceivedPacket.oSocketAddress = oClientAddress;
				oReceivedPacket.nPacketSize = nBytesReceived;

				if (!IsPacketAlreadyQueued(oReceivedPacket))
				{
					LOG_WARN(
						"Duplicate packet ignored | Type: {0} | RequestId: {1} | From: {2}",
						GetPacketName(oReceivedPacket.ePacketType),
						oReceivedPacket.nnRequestId,
						oReceivedPacket.oSocketAddress.GetIpAddress());
					continue;
				}

				memcpy(oReceivedPacket.oPacketPayload, messageBuffer, nBytesReceived);
				memset(messageBuffer, 0, nBytesReceived);

				const auto nnPendingPacketId = m_oPendingPacketsQueueManager.FindPendingPacket(oReceivedPacket);

				if (nnPendingPacketId.has_value())
					m_oPendingPacketsQueueManager.Remove(*nnPendingPacketId);

				m_oReceivedPacketsQueue.push_back(oReceivedPacket);
			}
		}

		void ProcessPackets()
		{
			for (const auto &oPendingPacket : m_oReceivedPacketsQueue)
			{
				if (!ProcessPacket(oPendingPacket))
					LOG_ERROR("Process packet failed: {0}", GetPacketName(oPendingPacket.ePacketType));
			}

			m_oReceivedPacketsQueue.clear();
		}

		void ProcessPendingPackets()
		{
			const std::vector<PendingPacket> &oPendingPackets = m_oPendingPacketsQueueManager.GetPacketsToRetry();

			for (const auto &oPendingPacket : oPendingPackets)
			{
				const char *pPacketData = oPendingPacket.oPacketSerializedData.data();
				const auto nPacketSizeInBytes = static_cast<int>(oPendingPacket.oPacketSerializedData.size());

				if (!SendPacketInternal(pPacketData, nPacketSizeInBytes))
				{
					// TODO LOG
				}

				LOG_DEBUG(
					"Packet sent for retry | Type: {0} | RequestId: {1} | Size: {2} bytes",
					GetPacketName(oPendingPacket.oPacketHeader.ePacketType),
					oPendingPacket.oPacketHeader.nnRequestId,
					nPacketSizeInBytes);
			}
		}

	private:
		bool ValidatePacketHeader(IN const PacketHeader &packetHeader, IN const int nBytesReceived)
		{
			const auto nPacketSize = sizeof(PacketHeader);

			if (nBytesReceived < nPacketSize)
			{
				LOG_ERROR("Incomplete packet received | Expected: {0} | Actual: {1}",
						  nPacketSize, nBytesReceived);

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

		std::vector<networking::ReceivedPacket> m_oReceivedPacketsQueue;

		PendingPacketsQueueManager m_oPendingPacketsQueueManager;

	private:
		__int64 m_nnNextRequestId = 1;
	};
}