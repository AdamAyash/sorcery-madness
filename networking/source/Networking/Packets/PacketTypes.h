#pragma once

namespace networking
{
	enum class PacketTypes : uint16_t
	{
		Undefined,
		Connect,
		ConnectionEstablished
	};

	inline const std::string GetPacketName(const PacketTypes ePacketType)
	{
		switch (ePacketType)
		{
		case PacketTypes::Connect:
			return "Connect";
		case PacketTypes::ConnectionEstablished:
			return "ConnectionEstablished";
		default:
			return "Not implemented packet type.";
		}
	}
}
