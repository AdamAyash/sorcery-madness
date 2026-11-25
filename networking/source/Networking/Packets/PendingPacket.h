#pragma once

#include "Packet.h"
#include <chrono>
#include <vector>
#include "../NetworkingGlobals.h"

namespace networking
{
	struct PendingPacket
	{
		PendingPacket()
		{
			nPacketRetryTimeoutInSeconds = n_MAX_PACKET_RETRY_TIMEOUT_IN_SECONDS;
		}

		PacketHeader oPacketHeader;
		short sRetryCount = 0;
		std::vector<char> oPacketSerializedData;
		int nPacketRetryTimeoutInSeconds = 0;
		std::chrono::steady_clock::time_point oLastSentTimesStamp;
	};
}