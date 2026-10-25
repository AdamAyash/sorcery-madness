#include "Server.h"
#include <Networking/Networking.h>
#include <Networking/Packets/PacketHeader.h>


Server::Server(const std::string& strServerIpAddress, const std::string& strPort)
	: m_strServerIpAddress(strServerIpAddress)
	, m_strPort(strPort)
{
}

Server::~Server()
{
}

bool Server::Start()
{
	common::Logger::Init();
	LOG_INFO("Server is starting...");

	m_oWinsockContext = common::CreateScope<networking::WinsockContext>();
	if (!m_oWinsockContext->Initialize())
	{
		LOG_ERROR("An error occured while initilizing winsock context");
		return false;
	}

	m_oServerSocket = common::CreateScope<networking::ServerSocket>(m_strServerIpAddress, m_strPort);
	if (!m_oServerSocket->Open())
	{
		LOG_ERROR("An error occured while opening the server socket");
		return false;
	}

	m_bIsRunning = true;

	LOG_INFO("Server successfully started on {0}:{1}", m_strServerIpAddress, m_strPort);

	return m_bIsRunning;
}

void Server::Run()
{
	while (m_bIsRunning)
	{
		RecievePackets();
		ProcessPackets();
		Update();
	}
}

void Server::RecievePackets()
{
	while (true)
	{
		char messageBuffer[networking::MAX_PACKET_SIZE]{};
		networking::SocketAddress oClientAddress;

		const auto nBytesRecived = m_oServerSocket->Recieve(messageBuffer, &oClientAddress);
		if (nBytesRecived == SOCKET_ERROR)
		{
			auto nNetworkError = WSAGetLastError();

			if (nNetworkError == WSAEWOULDBLOCK)
				break;

			LOG_ERROR("An error ocurred while recieving packets");
			return;
		}

		if (nBytesRecived < sizeof(networking::PacketHeader))
		{
			LOG_ERROR("The recieved packet is corrupted");
			return;
		}

		const networking::PacketHeader* const pPacketHeader 
			= reinterpret_cast<networking::PacketHeader*>(messageBuffer);

		networking::RecievedPacket oRecievedPacket;
		oRecievedPacket.oPacketHeader = *pPacketHeader;
		oRecievedPacket.oSocketAddress = oClientAddress;
		oRecievedPacket.nPacketSize = nBytesRecived;
		
		memcpy(oRecievedPacket.oPacketPayload, messageBuffer, nBytesRecived);
		memset(messageBuffer, 0, nBytesRecived);

		m_oRecivedPackets.push_back(oRecievedPacket);
	}
}

void Server::ProcessPackets()
{
	for (auto packetIterator = m_oRecivedPackets.begin(); packetIterator != m_oRecivedPackets.end(); ++packetIterator)
	{

	}
}

void Server::Update()
{
}
