#include "Socket.h"

namespace networking
{
	Socket::Socket()
	{
	}

	Socket::~Socket()
	{
		Close();
	}

	void Socket::Close()
	{
		if (m_oSocket != INVALID_SOCKET)
		{
			closesocket(m_oSocket);
			m_oSocket = INVALID_SOCKET;
		}
	}
}