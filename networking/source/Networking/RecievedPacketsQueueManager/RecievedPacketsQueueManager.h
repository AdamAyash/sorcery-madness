#pragma once

#include <Common/Common.h>
#include "../Packets/RecievedPacket.h"

namespace networking
{
	class RecievedPacketsQueueManager final : public common::NonCopyableObject
	{
	private:
		RecievedPacketsQueueManager();

	public:
		~RecievedPacketsQueueManager();

	public:
		static RecievedPacketsQueueManager& GetInstance();
		void Submit(IN const RecievedPacket oRecievedPacket);

	private:
		std::unordered_map<__int64, RecievedPackestList> m_oRevivedPacketsMap;
	};
}