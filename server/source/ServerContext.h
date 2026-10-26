#pragma once

#include "Common.h"
#include <Networking/Packets/RecievedPacket.h>

class ServerContext final : public common::NonCopyableObject
{
public:
	ServerContext();
	~ServerContext();

public:
	bool HandleClientConnectionRequest(const networking::RecievedPacket& oRecievedPacket);
};
