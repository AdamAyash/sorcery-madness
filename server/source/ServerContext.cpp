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

	const auto nSessionId = m_oSessionManager.CreateSession(oRecievedPacket.oSocketAddress);
	if (!nSessionId)
	{
		//TODO ERROR
		return false;
	}

	LOG_INFO("Client with session id: {0} and ip: {1} has connected to the server.", nSessionId,
		oRecievedPacket.oSocketAddress.GetIpAddress());

	oConnectionEstablishedPacket.nnRequestId = oRecievedPacket.nnRequestId;
	oConnectionEstablishedPacket.nPlayerSessionId = nSessionId;

	return true;
}