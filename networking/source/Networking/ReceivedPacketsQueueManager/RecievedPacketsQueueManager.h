#pragma once

#include <Common/Common.h>
#include "../Packets/ReceivedPacket.h"

namespace networking
{
	class ReceivedPacketsQueueManager final : public common::NonCopyableObject
	{
	private:
		ReceivedPacketsQueueManager();

	public:
		~ReceivedPacketsQueueManager();

	public:
		void Submit(IN const ReceivedPacket oReceivedPacket);

	private:
		std::unordered_map<__int64, ReceivedPacket> m_oReceivedPacketsMap;
	};
}