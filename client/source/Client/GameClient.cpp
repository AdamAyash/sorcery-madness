#include "GameClient.h"
#include <Networking/Version/ProtocolVersion.h>
#include <Common/Common.h>
#include <Networking/Packets/ConnectPacket.h>

GameClient::GameClient(const std::string& strServerIpAddress, const std::string& strPort)
	: NetworkEndpoint(strServerIpAddress, strPort)
{
}

GameClient::~GameClient()
{
}


bool GameClient::Start()
{
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

bool GameClient::IsPacketAlreadyQueued(IN const networking::RecievedPacket& oRecievedPacket) const
{
	for (auto nIndex = 0; nIndex < m_oRecivedPacketsQueue.size(); ++nIndex)
	{
		auto oCurrentPacket = m_oRecivedPacketsQueue[nIndex];
		if (oCurrentPacket.nnRequestId == oRecievedPacket.nnRequestId)
			return false;
	}

	return true;
}

bool GameClient::ConnectToServer()
{
	ASSERT(m_bIsRunning);

	LOG_INFO("Connecting to the server...");

	networking::ConnectPacket oConnectPacket;
	if (!SendPacket<networking::ConnectPacket>(oConnectPacket, nullptr, networking::PacketSendStrategy::Reliable))
	{
		LOG_ERROR("");
		return false;
	}

	m_eClientConnectionStatus = ClientConnectionStatuses::Connecting;

	return true;
}

void GameClient::Update()
{
	RecievePackets();
	ProcessPendingPackets();
	ProcessPackets();
}

const ClientConnectionStatuses GameClient::GetClientConnectionStatus() const
{
	return m_eClientConnectionStatus;
}

bool GameClient::ProcessPacket(const networking::RecievedPacket& oRecievedPacket)
{
	if (oRecievedPacket.ePacketType == networking::PacketTypes::ConnectionEstablished)
	{
		m_eClientConnectionStatus = ClientConnectionStatuses::Connected;
		LOG_INFO("Successfully connected to the server.");
	}

	return true;
}
