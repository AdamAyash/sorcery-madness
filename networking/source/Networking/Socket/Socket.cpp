#include "Socket.h"

namespace networking
{
	Socket::Socket(IN const std::string& strDomainAddress, IN const std::string& strPort)
		: BaseSocket(strDomainAddress, strPort)
	{
	}

	Socket::~Socket()
	{
	}
}