#include "Client.h"
#include <Networking/Version/ProtocolVersion.h>
#include <Common/Common.h>
#include <Networking/Packets/ConnectPacket.h>
#include "GameContext/GameContext.h"

Client::Client(const std::string &strServerIpAddress, const std::string &strPort, IN GameContext &oGameContext)
	: NetworkEndpoint(strServerIpAddress, strPort), m_oGameContext(oGameContext)
{
}

Client::~Client()
{
}

bool Client::Start()
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

	m_oClientPacketProcessor = common::CreateScope<ClientPacketProcessor>(m_oGameContext);

	m_bIsRunning = true;

	LOG_INFO("Client successfully started on {0}:{1}", m_strIpAddress, m_strPort);

	return m_bIsRunning;
}

bool Client::IsPacketAlreadyQueued(IN const networking::ReceivedPacket &oReceivedPacket) const
{
	for (auto nIndex = 0; nIndex < m_oRecivedPacketsQueue.size(); ++nIndex)
	{
		auto oCurrentPacket = m_oRecivedPacketsQueue[nIndex];
		if (oCurrentPacket.nnRequestId == oReceivedPacket.nnRequestId)
			return false;
	}

	return true;
}

bool Client::ConnectToServer()
{
	ASSERT(m_bIsRunning);

	LOG_INFO("Connecting to the server...");

	networking::ConnectPacket oConnectPacket;
	if (!SendPacket<networking::ConnectPacket>(oConnectPacket, nullptr, networking::PacketSendStrategy::Reliable))
	{
		// TODO LOG
		return false;
	}

	m_oGameContext.SetClientStatusAsConnecting();

	return true;
}

void Client::Update()
{
	RecievePackets();
	ProcessPendingPackets();
	ProcessPackets();
}

bool Client::ProcessPacket(const networking::ReceivedPacket &oReceivedPacket)
{
	if (!m_oClientPacketProcessor->ProcessPacket(oReceivedPacket))
	{
		// TODO LOG
		return false;
	}

	return true;
}
