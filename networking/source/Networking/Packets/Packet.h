#pragma once

#include "PacketHeader.h"

namespace networking
{
	struct Packet : public PacketHeader
	{
		Packet()
		{
			InitializeMemory();
		}
	};
}