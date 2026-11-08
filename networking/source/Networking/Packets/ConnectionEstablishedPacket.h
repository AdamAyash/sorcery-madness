#pragma once

#include "Packet.h"

namespace networking
{
	struct ConnectionEstablishedPacket : public Packet
	{
		ConnectionEstablishedPacket()
		{
			ePacketType = PacketTypes::ConnectionEstablished;
		}

		int nPlayerId = 0;
	};
}