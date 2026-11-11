#pragma once

#include <Common/Common.h>
#include "PacketHeader.h"
#include "../SocketAddress.h"
#include "../NetworkingGlobals.h"

namespace networking
{
	struct ReceivedPacket
	{
		ReceivedPacket()
		{
			InitializeMemory();
		}

		PacketTypes ePacketType;
		__int64 nnRequestId;
		SocketAddress oSocketAddress;
		int nPacketSize;
		char oPacketPayload[n_MAX_PACKET_SIZE];
	};

	using ReceivedPacketPacketsList = std::vector<ReceivedPacket>;
}