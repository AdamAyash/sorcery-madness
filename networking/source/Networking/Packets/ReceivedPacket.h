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
		}

		__int64 nnRequestId = 0;
		PacketTypes ePacketType = PacketTypes::Undefined;
		int nPacketSize = 0;

		SocketAddress oSocketAddress;
		char oPacketPayload[n_MAX_PACKET_SIZE]{};
	};

	using ReceivedPacketsList = std::vector<ReceivedPacket>;
}