#pragma once

#include <Common/Common.h>
#include <vector>
#include "../Packets/RecievedPacket.h"

namespace networking
{
	struct PendingPacket;

	class PendingPacketsQueueManager final : public common::NonCopyableObject
	{
	private:
		PendingPacketsQueueManager();

	public:
		~PendingPacketsQueueManager();

	public:
		static PendingPacketsQueueManager& GetInstance();

		void Submit(IN PendingPacket& oPendingPacket);
		void Remove(IN const __int64 nnPendingPacketId);

		const std::vector<PendingPacket>& GetPacketsToRetry();

		const __int64 FindPendingPacket(IN const RecievedPacket oRecievedPacket) const;

	private:
		std::unordered_map<__int64, PendingPacket> m_oPendingPacketsQueue;
		std::vector<PendingPacket> m_oPendingPacketsRegisteredForRetry;
	};
}