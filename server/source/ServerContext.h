#pragma once

#include "Common.h"
#include <Networking/Packets/RecievedPacket.h>

class ClientSession;

class ServerContext final : public common::NonCopyableObject
{
public:
	ServerContext();
	~ServerContext();

public:
	bool HandleClientConnectionRequest(const networking::RecievedPacket& oRecievedPacket);

private:


private:

	int m_nNextPlayerId = 1;
	std::unordered_map<int, ClientSession> m_oClientSessionsMap;
};
