#pragma once

#include "Common/Common.h"
#include <Networking/Packets/RecievedPacket.h>
#include <Networking/Packets/ConnectionEstablishedPacket.h>
#include <Networking/Packets/UpdateWorldStatePacket.h>

class Session;

class ServerContext final : public common::NonCopyableObject
{
public:
	ServerContext();
	~ServerContext();

public:
	void AddPlayer(IN const networking::PlayerStateData& oPlayerStateData);
	const networking::PlayersStateDataList& GetPlayerStateDataList() const;

private:
	networking::PlayersStateDataList m_oPlayersStateDataList;
};
