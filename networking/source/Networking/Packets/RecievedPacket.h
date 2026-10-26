#pragma once

#include <Common.h>
#include "PacketHeader.h"
#include "../Networking.h"
#include "../SocketAddress.h"

namespace networking
{
	struct RecievedPacket
	{
		RecievedPacket()
		{
			InitializeMemory();
		}

		PacketTypes ePacketType;
		SocketAddress oSocketAddress;
		int nPacketSize;
		char oPacketPayload[MAX_PACKET_SIZE];
	};
}