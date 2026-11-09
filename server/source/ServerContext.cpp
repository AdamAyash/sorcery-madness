#include "ServerContext.h"
#include <Networking/Networking.h>
#include <Networking/Packets/ConnectPacket.h>
#include <Common/Common.h>
#include "Sessions/Session.h"

ServerContext::ServerContext()
{
}

ServerContext::~ServerContext()
{
}

bool ServerContext::HandleClientConnectionRequest(IN const networking::RecievedPacket& oRecievedPacket, OUT networking::ConnectionEstablishedPacket& oConnectionEstablishedPacket)
{
	common::Scope<networking::ConnectPacket> oConnectPacket 
		= networking::DeserializePacket<networking::ConnectPacket>(oRecievedPacket);

	for (auto oClientSessionIterator = m_oClientSessionsMap.begin(); oClientSessionIterator != m_oClientSessionsMap.end(); ++oClientSessionIterator)
	{
		if (oClientSessionIterator->second.GetClientAddress() == oRecievedPacket.oSocketAddress)
		{
			LOG_WARN("Client is already connected to the server.");
			return false;
		}
	}

	const auto nSessionId = m_nNextSessionId;

	Session oClientSession(nSessionId, oRecievedPacket.oSocketAddress);
	m_oClientSessionsMap.try_emplace(nSessionId, oClientSession);

	LOG_INFO("Client with session id: {0} and ip: {1} has connected to the server.", nSessionId,
		oRecievedPacket.oSocketAddress.GetIpAddress());

	oConnectionEstablishedPacket.nnRequestId = oRecievedPacket.nnRequestId;
	oConnectionEstablishedPacket.nPlayerId = nSessionId;

	m_nNextSessionId++;

	return true;
}