#pragma once

namespace networking
{
	enum class PacketTypes : uint16_t
	{
		Undefined,
		Connect,
		ConnectionEstablished
	};
}
