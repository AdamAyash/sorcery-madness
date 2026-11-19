#pragma once

#include "Packet.h"

namespace networking
{
	struct PlayerStateData
	{
		PlayerStateData()
		{
			InitializeMemory();
		}

		int nSessionId;
		float positionX;
		float positionY;
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