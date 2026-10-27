#pragma once

#include "Packets/RecievedPacket.h"
#include <memory>

namespace networking
{
	template <class TPacket>
	const std::unique_ptr<TPacket> DeserializePacket(const RecievedPacket& oRecievedPacket)
	{
		static_assert(std::is_convertible_v<TPacket*, Packet*>,
			"TSocket must publicly inherit from BaseSocket");

		auto pPacket = std::make_unique<TPacket>();
		memcpy(pPacket, oRecievedPacket.oPacketPayload, sizeof(TPacket));

		return pPacket;
	}

	//Maximum packet size
	constexpr int MAX_PACKET_SIZE = 1024;
}