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
		}

		__int64 nnRequestId = 0;
		PacketRetryPolicy eRetryPolicy = PacketRetryPolicy::Limited;
		PacketTypes ePacketType = PacketTypes::Undefined;
	};
}
