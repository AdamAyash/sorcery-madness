#pragma once

#include "BaseSocket.h"

namespace networking
{
	class Socket final : public BaseSocket
	{
	public:
		Socket(IN const std::string& strDomainAddress, IN const std::string& strPort);
		virtual ~Socket();
	};
}