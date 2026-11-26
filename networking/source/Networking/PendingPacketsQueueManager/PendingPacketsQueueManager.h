#pragma once

#include <Common/Common.h>
#include <vector>

namespace networking
{
	struct PendingPacket;
	struct ReceivedPacket;

	class PendingPacketsQueueManager final : public common::NonCopyableObject
	{
	public:
		PendingPacketsQueueManager();

	public:
		~PendingPacketsQueueManager();

	public:
		void Submit(IN PendingPacket& oPendingPacket);
		void Remove(IN const __int64 nnPendingPacketId);

		const std::vector<PendingPacket>& GetPacketsToRetry();

		std::optional<__int64> FindPendingPacket(const ReceivedPacket& oPacket) const;

	private:
		std::unordered_map<__int64, PendingPacket> m_oPendingPacketsQueue;
		std::vector<PendingPacket> m_oPendingPacketsRegisteredForRetry;
	};
}