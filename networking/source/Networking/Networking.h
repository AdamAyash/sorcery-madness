#pragma once

#include "Packets/RecievedPacket.h"
#include "Packets/Packet.h"
#include <memory>

namespace networking
{
	template <class TPacket>
	std::unique_ptr<TPacket> DeserializePacket(IN const RecievedPacket& oRecievedPacket)
	{
		static_assert(std::is_convertible_v<TPacket*, Packet*>,
			"TPacket must publicly inherit from Packet");

		auto pPacket = std::make_unique<TPacket>();
		memcpy(pPacket.get(), oRecievedPacket.oPacketPayload, sizeof(TPacket));

		return pPacket;
	}

	template <class TPacket>
	const char* SerializePacket(IN const TPacket& oPacket)
	{
		static_assert(std::is_convertible_v<TPacket*, Packet*>,
			"TPacket must publicly inherit from Packet");

		const char* pPacketData = reinterpret_cast<const char*>(&oPacket);
		if (!pPacketData)
		{
			return nullptr;
		}

		return pPacketData;
	}
}