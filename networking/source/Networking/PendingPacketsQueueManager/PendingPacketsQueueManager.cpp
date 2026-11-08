#include "PendingPacketsQueueManager.h"
#include "../Packets/PendingPacket.h"
#include "../NetworkingGlobals.h"

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
		oPendingPacket.oLastSentTimesStamp = std::chrono::steady_clock::now();
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
				pendingPacketIterator = m_oPendingPacketsQueue.erase(pendingPacketIterator);
				LOG_WARN("Packet timed out");

				if (pendingPacketIterator == m_oPendingPacketsQueue.end())
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

	__int64 PendingPacketsQueueManager::FindPendingPacket(IN const RecievedPacket oRecievedPacket) const
	{
		for (auto pendingPacketIterator = m_oPendingPacketsQueue.begin(); pendingPacketIterator != m_oPendingPacketsQueue.end();)
		{
			if (pendingPacketIterator->second.oPacketHeader.nnRequestId == oRecievedPacket.nnRequestId)
				return pendingPacketIterator->first;
		}

		return -1;
	}
}