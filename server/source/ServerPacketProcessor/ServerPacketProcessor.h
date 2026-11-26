#pragma once

#include <Common/Common.h>
#include <Networking/Packets/ReceivedPacket.h>
#include "../Sessions/SessionManager.h"
#include <Networking/NetworkEndpoint.h>
#include <Networking/Socket/ServerSocket.h>

using ServerEndpoint = networking::NetworkEndpoint<networking::ServerSocket>;

class ServerContext;

class ServerPacketProcessor final : public common::NonCopyableObject
{
public:
	ServerPacketProcessor(IN ServerContext &oServerContext, IN ServerEndpoint &oServerEndpoint);
	~ServerPacketProcessor();

public:
	bool ProcessPacket(const networking::ReceivedPacket &oReceivedPacket);

private:
	bool ProcessClientConnectionRequest(IN const networking::ReceivedPacket &oReceivedPacket);

private:
	ServerContext &m_oServerContext;
	ServerEndpoint &m_oServerEndpoint;
	SessionManager &m_oSessionManager = SessionManager::GetInstance();
};