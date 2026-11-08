#include "Server.h"
#include <Networking/Networking.h>
#include <Networking/Version/ProtocolVersion.h>
#include <Networking/Packets/PacketHeader.h>
#include <Networking/Packets/ConnectionEstablishedPacket.h>


Server::Server(const std::string& strServerIpAddress, const std::string& strPort)
	: NetworkEndpoint(strServerIpAddress, strPort)
{
}

Server::~Server()
{
}

bool Server::Start()
{
	common::Logger::Init("Server instance");

	LOG_INFO("Protocol version: {0}", networking::ProtocolVersion::GetVersion());
	LOG_INFO("Server is starting...");

	m_oWinsockContext = common::CreateScope<networking::WinsockContext>();
	if (!m_oWinsockContext->Initialize())
	{
		LOG_ERROR("An error occured while initilizing winsock context");
		return false;
	}

	m_oSocket = common::CreateScope<networking::ServerSocket>(m_strIpAddress, m_strPort);
	if (!m_oSocket->Open())
	{
		LOG_ERROR("An error occured while opening the server socket");
		return false;
	}

	m_oServerContext = common::CreateScope<ServerContext>();

	m_bIsRunning = true;

	LOG_INFO("Server successfully started on {0}:{1}", m_strIpAddress, m_strPort);

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

bool Server::ProcessPacket(const networking::RecievedPacket& oRecievedPacket)
{
	switch (oRecievedPacket.ePacketType)
	{
	case networking::PacketTypes::Connect:
	{
		networking::ConnectionEstablishedPacket oConnectionEstablishedPacket;

		if (!m_oServerContext->HandleClientConnectionRequest(oRecievedPacket, oConnectionEstablishedPacket))
		{
			//TODO LOG
			return false;
		}

		if (!SendPacket<networking::ConnectionEstablishedPacket>(oConnectionEstablishedPacket, &oRecievedPacket.oSocketAddress))
		{
			//TODO LOG
			return false;
		}

		return true;
	}

	default:
		LOG_ERROR("Unsupported packet type: ", static_cast<int>(oRecievedPacket.ePacketType));
		return false;
	}
}

void Server::Update()
{
}
