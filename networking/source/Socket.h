#pragma once

#include <winsock2.h>
#include <Common.h>

namespace networking
{
	class Socket final : public common::NonCopyableObject
	{
	public:
		Socket();
		~Socket();

	public:
		void Close();
		

	private:
		SOCKET m_oSocket = INVALID_SOCKET;
	};
}