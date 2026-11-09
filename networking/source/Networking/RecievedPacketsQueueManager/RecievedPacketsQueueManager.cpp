#include "RecievedPacketsQueueManager.h"

namespace networking
{
	RecievedPacketsQueueManager::RecievedPacketsQueueManager()
	{
	}

	RecievedPacketsQueueManager::~RecievedPacketsQueueManager()
	{
	}

	RecievedPacketsQueueManager& RecievedPacketsQueueManager::GetInstance()
	{
		static RecievedPacketsQueueManager oRecievedPacketsQueueManager;
		return oRecievedPacketsQueueManager;
	}
}