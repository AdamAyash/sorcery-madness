#include "WinsockContext.h"
#include <winsock2.h>
#include <Common.h>

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
			//TODO LOG
			return false;
		}

		return true;
	}
}