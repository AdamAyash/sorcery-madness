#include "ServerSocket.h"

namespace networking
{
	ServerSocket::ServerSocket()
	{
	}

	ServerSocket::~ServerSocket()
	{
		Close();
	}

	void ServerSocket::Close()
	{
		if (m_oSocket != INVALID_SOCKET)
		{
			closesocket(m_oSocket);
			m_oSocket = INVALID_SOCKET;
		}
	}
}