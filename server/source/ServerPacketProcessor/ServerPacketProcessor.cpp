#include "ServerPacketProcessor.h"
#include "../ServerContext/ServerContext.h"
#include <Networking/Packets/ConnectPacket.h>
#include <Networking/Networking.h>

ServerPacketProcessor::ServerPacketProcessor(IN ServerContext& oServerContext, IN ServerEndpoint& oServerEndpoint)
	: m_oServerContext(oServerContext)
	, m_oServerEndpoint(oServerEndpoint)
{
}

ServerPacketProcessor::~ServerPacketProcessor()
{
}

bool ServerPacketProcessor::ProcessPacket(const networking::ReceivedPacket& oRecievedPacket)
{
	switch (oRecievedPacket.ePacketType)
	{
	case networking::PacketTypes::Connect:
		return ProcessClientConnectionRequest(oRecievedPacket);

	default:
		LOG_ERROR("Unsupported packet type: ", static_cast<int>(oRecievedPacket.ePacketType));
		return false;
	}

	return true;
}

bool ServerPacketProcessor::ProcessClientConnectionRequest(IN const networking::ReceivedPacket& oRecievedPacket)
{
	common::Scope<networking::ConnectPacket> oConnectPacket
		= networking::DeserializePacket<networking::ConnectPacket>(oRecievedPacket);

	const auto nSessionId = m_oSessionManager.CreateSession(oRecievedPacket.oSocketAddress);
	if (nSessionId == -1)
	{
		//TODO ERROR
		return false;
	}

	LOG_INFO("Client with session id: {0} and ip: {1} has connected to the server.", nSessionId,
		oRecievedPacket.oSocketAddress.GetIpAddress());

	networking::ConnectionEstablishedPacket oConnectionEstablishedPacket;
	oConnectionEstablishedPacket.nnRequestId = oRecievedPacket.nnRequestId;
	oConnectionEstablishedPacket.nPlayerSessionId = nSessionId;

	networking::PlayerStateData oPlayerStateData;
	oPlayerStateData.nSessionId = nSessionId;
	oPlayerStateData.positionX = rand() % 1270;
	oPlayerStateData.positionX = rand() % 720;

	m_oServerContext.AddPlayer(oPlayerStateData);

	if (!m_oServerEndpoint.SendPacket(oConnectionEstablishedPacket, &oRecievedPacket.oSocketAddress,
		networking::PacketSendStrategy::Reliable))
	{
		//TODO LOG
		return false;
	}

	return true;
}