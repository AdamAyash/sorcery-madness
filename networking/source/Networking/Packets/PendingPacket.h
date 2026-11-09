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
			InitializeMemory();

			nPacketRetryTimeoutInSeconds = n_MAX_PACKET_RETRY_TIMEOUT_IN_SECONDS;
		}

		Packet oPacketHeader;
		short sRetryCount;
		std::vector<char> oPacketSerializedData;
		int nPacketRetryTimeoutInSeconds;
		std::chrono::steady_clock::time_point oLastSentTimesStamp;
	};
}