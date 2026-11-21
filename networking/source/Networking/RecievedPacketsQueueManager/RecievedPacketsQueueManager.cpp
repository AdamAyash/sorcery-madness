#include "RecievedPacketsQueueManager.h"

namespace networking
{
	ReceivedPacketsQueueManager::ReceivedPacketsQueueManager()
	{
	}

	ReceivedPacketsQueueManager::~ReceivedPacketsQueueManager()
	{
	}

	ReceivedPacketsQueueManager& ReceivedPacketsQueueManager::GetInstance()
	{
		static ReceivedPacketsQueueManager oReceivedPacketsQueueManager;
		return oReceivedPacketsQueueManager;
	}
}