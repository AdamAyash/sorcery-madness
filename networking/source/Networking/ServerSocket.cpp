#include "ServerSocket.h"

namespace networking
{
	ServerSocket::ServerSocket(const std::string& strDomainAddress, const std::string& strPort)
		: BaseSocket(strDomainAddress, strPort)
	{
	}

	ServerSocket::~ServerSocket()
	{
	}

	bool ServerSocket::Open()
	{
		if (!BASE::Open())
		{
			//TODO LOG
			return false;
		}

		if (SOCKET_ERROR == bind(m_oSocket, &m_oSocketAddress.Get(), m_oSocketAddress.Size()))
		{
			LOG_ERROR("An error occured while binding the socket on {0}:{1}", m_strDomainAddress, m_strPort);
			return false;
		}

		return true;
	}
}