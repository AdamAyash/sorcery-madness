#pragma once

namespace networking
{
	template <class TPacket>
	TPacket* DeserializePacket()
	{
		return nullptr;
	}

	//Maximum packet size
	constexpr int MAX_PACKET_SIZE = 1024;
}