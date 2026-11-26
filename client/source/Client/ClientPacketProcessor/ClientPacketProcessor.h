#pragma once

#include <Common/Common.h>
#include <Networking/Packets/ReceivedPacket.h>

class GameContext;

class ClientPacketProcessor final : public common::NonCopyableObject
{
public:
	ClientPacketProcessor(IN GameContext &oGameContext);
	~ClientPacketProcessor();

public:
	bool ProcessPacket(const networking::ReceivedPacket &oReceivedPacket);

private:
	bool ProcessConnectionEstablishedRequest(IN const networking::ReceivedPacket &oReceivedPacket);
	bool ProcessUpdateWorldStateRequest(IN const networking::ReceivedPacket &oReceivedPacket);

private:
	GameContext &m_oGameContext;
};