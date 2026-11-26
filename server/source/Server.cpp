#include "Server.h"
#include <Networking/Networking.h>
#include <Networking/Version/ProtocolVersion.h>
#include <Networking/Packets/PacketHeader.h>
#include <Networking/Packets/ReceivedPacket.h>
#include <Networking/Packets/ConnectionEstablishedPacket.h>
#include "ServerContext/ServerContext.h"
#include <Networking/Packets/UpdateWorldStatePacket.h>
#include "Sessions/Session.h"
#include <chrono>
#include "Sessions/SessionManager.h"

constexpr auto SERVER_TICK_RATE = std::chrono::duration<double>(1.0 / 60.0);
constexpr auto WORLD_STATE_RATE = std::chrono::duration_cast<common::Clock::duration>(std::chrono::duration<double>(1.0 / 10.0));

Server::Server(const std::string &strServerIpAddress, const std::string &strPort)
	: NetworkEndpoint(strServerIpAddress, strPort)
{
}

Server::~Server()
{
}

bool Server::Start()
{
	common::Timer oTimer;
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
	m_oServerPacketProcessor = common::CreateScope<ServerPacketProcessor>(*m_oServerContext, *static_cast<ServerEndpoint *>(this));

	m_bIsRunning = true;

	LOG_INFO("Server successfully started on {0}:{1}", m_strIpAddress, m_strPort);
	LOG_INFO("Server start took {0}ms", oTimer.ElapsedMilliseconds());

	return m_bIsRunning;
}

bool Server::IsPacketAlreadyQueued(IN const networking::ReceivedPacket &oReceivedPacket) const
{
	for (auto nIndex = 0; nIndex < m_oRecivedPacketsQueue.size(); ++nIndex)
	{
		auto oCurrentPacket = m_oRecivedPacketsQueue[nIndex];
		if (oCurrentPacket.nnRequestId == oReceivedPacket.nnRequestId &&
			oCurrentPacket.oSocketAddress == oReceivedPacket.oSocketAddress)
		{
			return false;
		}
	}

	return true;
}

void Server::Run()
{
	auto previousTime = common::Clock::now();

	auto nextSimulationTick = previousTime;
	auto nextWorldStateUpdate = previousTime;

	while (m_bIsRunning)
	{
		const auto currentTime = common::Clock::now();

		RecievePackets();
		ProcessPackets();

		if (currentTime >= nextWorldStateUpdate)
		{
			Update();
			nextWorldStateUpdate += WORLD_STATE_RATE;
		}
	}
}

bool Server::ProcessPacket(const networking::ReceivedPacket &oReceivedPacket)
{
	if (!m_oServerPacketProcessor->ProcessPacket(oReceivedPacket))
	{
		// TODO LOG
		return false;
	}

	return true;
}

void Server::Update()
{
	const auto &playerStateDataList = m_oServerContext->GetPlayerStateDataList();
	const auto nPlayerCount = playerStateDataList.size();

	if (nPlayerCount <= 0)
		return;

	networking::UpdateWorldStatePacket oUpdateWorldStatePacket;
	oUpdateWorldStatePacket.nPlayerCount = nPlayerCount;

	for (int nIndex = 0; nIndex < nPlayerCount; ++nIndex)
		oUpdateWorldStatePacket.playerStateDataArray[nIndex] = playerStateDataList[nIndex];

	for (int index = 0; index < nPlayerCount; ++index)
	{
		const auto nSessionId = oUpdateWorldStatePacket.playerStateDataArray[index].nSessionId;
		const auto oClientAddress = SessionManager::GetInstance().GetSesion(nSessionId).GetClientAddress();

		if (!SendPacket<networking::UpdateWorldStatePacket>(oUpdateWorldStatePacket, &oClientAddress))
		{
			LOG_ERROR("An error occured while updating world state.");
		}
	}
}
