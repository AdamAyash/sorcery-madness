#pragma once

#include "Packet.h"

namespace networking
{
	struct ConnectPacket : public Packet
	{
		ConnectPacket()
		{
			ePacketType = networking::PacketTypes::Connect;
			eRetryPolicy = PacketRetryPolicy::Unlimited;
		}
	};
}