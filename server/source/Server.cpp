#include "Server.h"
#include <Networking/Networking.h>
#include <Networking/Packets/PacketHeader.h>


Server::Server(const std::string& strServerIpAddress, const std::string& strPort)
	: NetworkEndpoint(strServerIpAddress, strPort)
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
	case networking::PacketTypes::PacketTypeConnect:
		return m_oServerContext->HandleClientConnectionRequest(oRecievedPacket);

	default:
		return false;
	}
}

void Server::Update()
{
}
