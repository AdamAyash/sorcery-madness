#pragma once

#include "Packets/ReceivedPacket.h"
#include "Packets/Packet.h"
#include <memory>

namespace networking
{
	template <class TPacket, class = class std::enable_if<std::is_base_of_v<Packet, TPacket>>>
	std::unique_ptr<TPacket> DeserializePacket(IN const ReceivedPacket &oReceivedPacket)
	{

		auto pPacket = std::make_unique<TPacket>();
		memcpy(pPacket.get(), oReceivedPacket.oPacketPayload, sizeof(TPacket));

		return pPacket;
	}

	template <class TPacket, class = class std::enable_if<std::is_base_of_v<Packet, TPacket>>>
	const char *SerializePacket(IN const TPacket &oPacket)
	{
		const char *pPacketData = reinterpret_cast<const char *>(&oPacket);
		if (!pPacketData)
		{
			return nullptr;
		}

		return pPacketData;
	}
}