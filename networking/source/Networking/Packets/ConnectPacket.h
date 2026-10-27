#pragma once

#include "Packet.h"

namespace networking
{
	struct ConnectPacket : public Packet
	{
		ConnectPacket()
		{
			InitializeMemory();
		}
	};
}