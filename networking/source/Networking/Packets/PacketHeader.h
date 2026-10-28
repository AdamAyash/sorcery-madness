#pragma once

#include <Common/Common.h>
#include "PacketTypes.h"

namespace networking
{
	struct PacketHeader
	{
		PacketHeader()
		{
			InitializeMemory();
		}

		__int64 nnRequestId = 0;
		PacketTypes ePacketType;
	};
}
