#pragma once

#include "Common/Common.h"
#include <Networking/Packets/RecievedPacket.h>
#include <Networking/Packets/ConnectionEstablishedPacket.h>

class Session;

class ServerContext final : public common::NonCopyableObject
{
public:
	ServerContext();
	~ServerContext();

public:
	bool HandleClientConnectionRequest(IN const networking::RecievedPacket& oRecievedPacket, 
		OUT networking::ConnectionEstablishedPacket& oConnectionEstablishedPacket);

private:
	int m_nNextSessionId = 1;
	std::unordered_map<int, Session> m_oClientSessionsMap;
};
