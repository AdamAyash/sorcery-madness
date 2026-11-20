#pragma once

#include <Common/Common.h>
#include "../Packets/RecievedPacket.h"

namespace networking
{
	class ReceivedPacketsQueueManager final : public common::NonCopyableObject
	{
	private:
		ReceivedPacketsQueueManager();

	public:
		~ReceivedPacketsQueueManager();

	public:
		static ReceivedPacketsQueueManager &GetInstance();
		void Submit(IN const ReceivedPacket oRecievedPacket);

	private:
		std::unordered_map<__int64, ReceivedPacket> m_oRevivedPacketsMap;
	};
}