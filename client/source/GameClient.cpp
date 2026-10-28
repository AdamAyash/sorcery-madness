#include "GameClient.h"
#include <Networking/Version/ProtocolVersion.h>

GameClient::GameClient(const std::string& strServerIpAddress, const std::string& strPort)
	: NetworkEndpoint(strServerIpAddress, strPort)
{
}

GameClient::~GameClient()
{
}

bool GameClient::Start()
{
	common::Logger::Init("Game client");

	LOG_INFO("Protocol version: {0}", networking::ProtocolVersion::GetVersion());
	LOG_INFO("Client is starting...");

	m_oWinsockContext = common::CreateScope<networking::WinsockContext>();
	if (!m_oWinsockContext->Initialize())
	{
		LOG_ERROR("An error occured while initilizing winsock context");
		return false;
	}

	m_oSocket = common::CreateScope<networking::Socket>(m_strIpAddress, m_strPort);
	if (!m_oSocket->Open())
	{
		LOG_ERROR("An error occured while opening the client socket");
		return false;
	}

	m_bIsRunning = true;

	LOG_INFO("Client successfully started on {0}:{1}", m_strIpAddress, m_strPort);

	return m_bIsRunning;
}

bool GameClient::ProcessPacket(const networking::RecievedPacket& oRecievedPacket)
{
	return true;
}
