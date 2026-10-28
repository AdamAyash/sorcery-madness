#pragma once

#include <Common/Common.h>
#include "PacketHeader.h"
#include "../SocketAddress.h"
#include "../NetworkingGlobals.h"

namespace networking
{
	struct RecievedPacket
	{
		RecievedPacket()
		{
			InitializeMemory();
		}

		PacketTypes ePacketType;
		__int64 nnRequestId;
		SocketAddress oSocketAddress;
		int nPacketSize;
		char oPacketPayload[n_MAX_PACKET_SIZE];
	};
}