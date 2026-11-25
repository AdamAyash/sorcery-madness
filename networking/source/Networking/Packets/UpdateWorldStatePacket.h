#pragma once

#include "Packet.h"
#include "../NetworkingGlobals.h"

namespace networking
{
	struct PlayerStateData
	{
		PlayerStateData()
		{
		}

		int nSessionId = 0;
		float positionX = 0.f;
		float positionY = 0.f;
	};

	using PlayersStateDataList = std::vector<PlayerStateData>;

	struct UpdateWorldStatePacket : public Packet
	{
		UpdateWorldStatePacket()
		{
			ePacketType = PacketTypes::UpdateWorldState;
		}

		int nPlayerCount = 0;
		PlayerStateData playerStateDataArray[n_MAX_PLAYER_SESSIONS_SUPPORTED];
	};
}