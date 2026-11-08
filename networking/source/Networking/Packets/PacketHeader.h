#pragma once

#include <Common/Common.h>
#include "PacketTypes.h"
#include "PacketRetryPolicty.h"

namespace networking
{
	struct PacketHeader
	{
		PacketHeader()
		{
			InitializeMemory();
		}

		__int64 nnRequestId;
		PacketRetryPolicy eRetryPolicy;
		PacketTypes ePacketType;
	};
}
