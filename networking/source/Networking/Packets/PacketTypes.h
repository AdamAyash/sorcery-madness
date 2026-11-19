#pragma once

namespace networking
{
	enum class PacketTypes : uint16_t
	{
		Undefined,
		Connect,
		ConnectionEstablished,
		UpdateWorldState,
	};

	inline const std::string GetPacketName(const PacketTypes ePacketType)
	{
		switch (ePacketType)
		{
		case PacketTypes::Connect:
			return "Connect";
		case PacketTypes::ConnectionEstablished:
			return "ConnectionEstablished";
		case PacketTypes::UpdateWorldState:
			return "UpdateWorldState";
		default:
			return "Not implemented packet type.";
		}
	}
}
