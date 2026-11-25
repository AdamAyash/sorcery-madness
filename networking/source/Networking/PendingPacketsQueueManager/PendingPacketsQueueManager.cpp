#include "PendingPacketsQueueManager.h"
#include "../Packets/PendingPacket.h"
#include "../Packets/ReceivedPacket.h"
#include "../NetworkingGlobals.h"
#include <optional>

namespace networking
{
	PendingPacketsQueueManager::PendingPacketsQueueManager()
	{
	}

	PendingPacketsQueueManager::~PendingPacketsQueueManager()
	{
	}

	PendingPacketsQueueManager& PendingPacketsQueueManager::GetInstance()
	{
		static PendingPacketsQueueManager oPendingPacketsManager;
		return oPendingPacketsManager;
	}

	void PendingPacketsQueueManager::Submit(IN PendingPacket& oPendingPacket)
	{
		oPendingPacket.oLastSentTimesStamp = common::Clock::now();
		m_oPendingPacketsQueue[oPendingPacket.oPacketHeader.nnRequestId] = oPendingPacket;
	}

	void PendingPacketsQueueManager::Remove(IN const __int64 nnPendingPacketId)
	{
		m_oPendingPacketsQueue.erase(nnPendingPacketId);
	}

	const std::vector<PendingPacket>& PendingPacketsQueueManager::GetPacketsToRetry()
	{
		m_oPendingPacketsRegisteredForRetry.clear();

		for (auto pendingPacketIterator = m_oPendingPacketsQueue.begin(); 
			pendingPacketIterator != m_oPendingPacketsQueue.end();)
		{
			if (pendingPacketIterator->second.sRetryCount >= n_MAX_PACKET_RETRY_COUNT)
			{
				LOG_WARN("Packet #{0} dropped after {1} retries", pendingPacketIterator->first, n_MAX_PACKET_RETRY_COUNT);
				pendingPacketIterator = m_oPendingPacketsQueue.erase(pendingPacketIterator);
				continue;
			}

			const auto oCurrentTimeStamp = std::chrono::steady_clock::now();
			const std::chrono::seconds oPacketRetryTimeoutInSeconds(pendingPacketIterator->second.nPacketRetryTimeoutInSeconds);

			if (oCurrentTimeStamp - pendingPacketIterator->second.oLastSentTimesStamp >= oPacketRetryTimeoutInSeconds)
			{
				if (pendingPacketIterator->second.oPacketHeader.eRetryPolicy == PacketRetryPolicy::Unlimited)
				{
					pendingPacketIterator->second.nPacketRetryTimeoutInSeconds += 2;
				}
				else 
				{
					pendingPacketIterator->second.sRetryCount++;
				}

				pendingPacketIterator->second.oLastSentTimesStamp = oCurrentTimeStamp;
				m_oPendingPacketsRegisteredForRetry.push_back(pendingPacketIterator->second);
			}

			pendingPacketIterator++;
		}

		return m_oPendingPacketsRegisteredForRetry;
	}

	std::optional<__int64> PendingPacketsQueueManager::FindPendingPacket(const ReceivedPacket& oPacket) const
	{
		const auto it = m_oPendingPacketsQueue.find(oPacket.nnRequestId);
		return it != m_oPendingPacketsQueue.end() ? std::optional(it->first) : std::nullopt;
	}
}