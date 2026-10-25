#pragma once

#include <Common.h>
#include "PacketTypes.h"

namespace networking
{
	struct PacketHeader
	{
		PacketHeader()
		{
			InitializeMemory();
		}

		PacketTypes ePacketType;
	};
}
