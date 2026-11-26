#include "ServerPacketProcessor.h"
#include "../ServerContext/ServerContext.h"
#include <Networking/Packets/ConnectPacket.h>
#include <Networking/Networking.h>

ServerPacketProcessor::ServerPacketProcessor(IN ServerContext &oServerContext, IN ServerEndpoint &oServerEndpoint)
	: m_oServerContext(oServerContext), m_oServerEndpoint(oServerEndpoint)
{
}

ServerPacketProcessor::~ServerPacketProcessor()
{
}

bool ServerPacketProcessor::ProcessPacket(const networking::ReceivedPacket &oReceivedPacket)
{
	switch (oReceivedPacket.ePacketType)
	{
	case networking::PacketTypes::Connect:
		return ProcessClientConnectionRequest(oReceivedPacket);

	default:
		LOG_ERROR("Unsupported packet type: ", static_cast<int>(oReceivedPacket.ePacketType));
		return false;
	}
}

bool ServerPacketProcessor::ProcessClientConnectionRequest(IN const networking::ReceivedPacket &oReceivedPacket)
{
	common::Scope<networking::ConnectPacket> oConnectPacket = networking::DeserializePacket<networking::ConnectPacket>(oReceivedPacket);

	const auto oSessionId = m_oSessionManager.CreateSession(oReceivedPacket.oSocketAddress);
	if (!oSessionId.has_value())
	{
		// TODO ERROR
		return false;
	}

	LOG_INFO("Client with session id: {0} and ip: {1} has connected to the server.", oSessionId.value(),
			 oReceivedPacket.oSocketAddress.GetIpAddress());

	networking::ConnectionEstablishedPacket oConnectionEstablishedPacket;
	oConnectionEstablishedPacket.nnRequestId = oReceivedPacket.nnRequestId;
	oConnectionEstablishedPacket.nPlayerSessionId = oSessionId.value();

	networking::PlayerStateData oPlayerStateData;
	oPlayerStateData.nSessionId = oSessionId.value();
	oPlayerStateData.positionX = rand() % 1270;
	oPlayerStateData.positionX = rand() % 720;

	m_oServerContext.AddPlayer(oPlayerStateData);

	if (!m_oServerEndpoint.SendPacket(oConnectionEstablishedPacket, &oReceivedPacket.oSocketAddress,
									  networking::PacketSendStrategy::Reliable))
	{
		// TODO LOG
		return false;
	}

	return true;
}