#include "ServerContext.h"
#include <Networking/Networking.h>
#include <Networking/Packets/ConnectPacket.h>
#include <Common/Common.h>
#include "ClientSession.h"


ServerContext::ServerContext()
{
}

ServerContext::~ServerContext()
{
}

bool ServerContext::HandleClientConnectionRequest(const networking::RecievedPacket& oRecievedPacket)
{
	common::Scope<networking::ConnectPacket> oConnectPacket 
		= networking::DeserializePacket<networking::ConnectPacket>(oRecievedPacket);

	const auto nPlayerId = m_nNextPlayerId;
	ClientSession oClientSession(nPlayerId, oRecievedPacket.oSocketAddress);

	m_oClientSessionMap.try_emplace(nPlayerId, oClientSession);

	LOG_INFO("Client with id: {0} and ip: {1} has connected to the server.", nPlayerId, 
		oRecievedPacket.oSocketAddress.GetIpAddress());

	m_nNextPlayerId++;

	return true;
}