#pragma once

#include <Common/Common.h>
#include <Networking/Packets/RecievedPacket.h>

class GameContext;

class ClientPacketProcessor final : public common::NonCopyableObject
{
public:
	ClientPacketProcessor(IN GameContext& oGameContext);
	~ClientPacketProcessor();

public:
	bool ProcessPacket(const networking::RecievedPacket& oRecievedPacket);

private:
	bool ProcessConnectionEstablishedRequest(IN const networking::RecievedPacket& oRecievedPacket);
	bool ProcessUpdateWorldStateRequest(IN const networking::RecievedPacket& oRecievedPacket);

private:
	GameContext& m_oGameContext;
};