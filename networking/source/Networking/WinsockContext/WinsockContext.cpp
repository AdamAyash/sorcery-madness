#include "WinsockContext.h"
#include <winsock2.h>
#include <Common/Common.h>

namespace networking
{
	WinsockContext::WinsockContext()
	{
	}

	WinsockContext::~WinsockContext()
	{
		WSACleanup();
	}

	bool WinsockContext::Initialize()
	{
		WSADATA oWSAData;
		if (WSAStartup(MAKEWORD(2, 2), &oWSAData) != 0)
		{
			LOG_ERROR("Failed to initialize winsock2");
			return false;
		}

		return true;
	}
}