#pragma once

#include "Packets/RecievedPacket.h"
#include "Packets/Packet.h"
#include <memory>

namespace networking
{
	template <class TPacket>
	std::unique_ptr<TPacket> DeserializePacket(const RecievedPacket& oRecievedPacket)
	{
		static_assert(std::is_convertible_v<TPacket*, Packet*>,
			"TPacket must publicly inherit from Packet");

		auto pPacket = std::make_unique<TPacket>();
		memcpy(pPacket.get(), oRecievedPacket.oPacketPayload, sizeof(TPacket));

		return pPacket;
	}
}