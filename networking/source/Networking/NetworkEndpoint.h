#pragma once

#include <Common/Common.h>
#include "Networking.h"
#include "Networking/BaseSocket.h"
#include "Packets/PacketHeader.h"
#include "WinsockContext.h"
#include <vector>
#include <type_traits>

namespace networking
{
	template <class TSocket>
	class NetworkEndpoint : public common::NonCopyableObject
	{
		static_assert(std::is_convertible_v<TSocket*, BaseSocket*>,
			"TSocket must publicly inherit from BaseSocket");

	protected:
		NetworkEndpoint(IN const std::string& strIpAddress, IN const std::string& strPort)
			: m_strIpAddress(strIpAddress)
			, m_strPort(strPort)
		{
		}

	public:
		virtual ~NetworkEndpoint()
		{
		}

	public:
		virtual bool Start() PURE;
		virtual bool ProcessPacket(IN const networking::RecievedPacket& oRecievedPacket) PURE;

		template<class TPacket>
		bool SendPacket(IN const TPacket& oPacket)
		{
			static_assert(std::is_convertible_v<TPacket*, Packet*>,
				"TPacket must publicly inherit from Packet");

			const char* pPacket = reinterpret_cast<const char*>(&oPacket);
			if (!pPacket)
			{
				//TODO ERROR
				return false;
			}

			const auto nBytesSent = m_oSocket->Send(pPacket, sizeof(TPacket));
			LOG_DEBUG("Sent {0} bytes", nBytesSent);

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

					LOG_ERROR("An error ocurred while recieving packets");
					return;
				}

				const PacketHeader* const pPacketHeader 
					= reinterpret_cast<PacketHeader*>(messageBuffer);

				if (!pPacketHeader)
					continue;

				if (!ValidatePacketHeader(*pPacketHeader, nBytesRecived))
				{
					LOG_ERROR("The recieved packet is corrupted");
					continue;
				}

				if (!ValidateIfPacketDoesntExists(*pPacketHeader))
				{
					LOG_WARN("The recieved packet is already registered");
					continue;
				}

				RecievedPacket oRecievedPacket;
				oRecievedPacket.nnRequestId = pPacketHeader->nnRequestId;
				oRecievedPacket.ePacketType = pPacketHeader->ePacketType;
				oRecievedPacket.oSocketAddress = oClientAddress;
				oRecievedPacket.nPacketSize = nBytesRecived;

				memcpy(oRecievedPacket.oPacketPayload, messageBuffer, nBytesRecived);
				memset(messageBuffer, 0, nBytesRecived);

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

	private:
		bool ValidatePacketHeader(IN const PacketHeader& packetHeader, IN const int nBytesRecived)
		{
			if (nBytesRecived < sizeof(PacketHeader))
			{
				LOG_ERROR("The recieved packet is corrupted");
				return false;
			}

			return true;
		}

		bool ValidateIfPacketDoesntExists(IN const PacketHeader& packetHeader)
		{
			for (auto nIndex = 0; nIndex < m_oRecivedPacketsQueue.size(); ++nIndex)
			{
				auto oCurrentPacket = m_oRecivedPacketsQueue[nIndex];
				if (oCurrentPacket.nnRequestId == packetHeader.nnRequestId)
					return false;
			}

			return true;
		}

	protected:
		const std::string m_strIpAddress;
		const std::string m_strPort;

		common::Scope <networking::WinsockContext> m_oWinsockContext = nullptr;
		common::Scope<TSocket> m_oSocket = nullptr;

		std::vector<networking::RecievedPacket> m_oRecivedPacketsQueue;

		__int64 m_nnNextRequestId = 1;
	};
}