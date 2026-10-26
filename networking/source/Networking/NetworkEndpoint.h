#pragma once

#include "Common.h"
#include "Networking.h"
#include "Packets/PacketHeader.h"
#include "WinsockContext.h"
#include <vector>

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
		virtual bool ProcessPacket(const networking::RecievedPacket& oRecievedPacket) PURE;

	protected:
		void RecievePackets()
		{
			while (true)
			{
				char messageBuffer[MAX_PACKET_SIZE]{};
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

				if (nBytesRecived < sizeof(PacketHeader))
				{
					LOG_ERROR("The recieved packet is corrupted");
					return;
				}

				const PacketHeader* const pPacketHeader
					= reinterpret_cast<PacketHeader*>(messageBuffer);

				RecievedPacket oRecievedPacket;
				oRecievedPacket.ePacketType = pPacketHeader->ePacketType;
				oRecievedPacket.oSocketAddress = oClientAddress;
				oRecievedPacket.nPacketSize = nBytesRecived;

				memcpy(oRecievedPacket.oPacketPayload, messageBuffer, nBytesRecived);
				memset(messageBuffer, 0, nBytesRecived);

				m_oRecivedPackets.push_back(oRecievedPacket);
			}
		}

		void ProcessPackets()
		{
			for (auto packetIterator = m_oRecivedPackets.begin(); packetIterator != m_oRecivedPackets.end();)
			{
				const auto ePacketType = packetIterator->ePacketType;
				if (!ProcessPacket(*packetIterator))
				{
					++packetIterator;
					continue;
				}

				packetIterator = m_oRecivedPackets.erase(packetIterator);
			}
		}

	protected:
		const std::string m_strIpAddress;
		const std::string m_strPort;

		common::Scope <networking::WinsockContext> m_oWinsockContext = nullptr;
		common::Scope<TSocket> m_oSocket = nullptr;

		std::vector<networking::RecievedPacket> m_oRecivedPackets;
	};
}