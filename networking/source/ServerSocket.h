#pragma once

#include <winsock2.h>
#include <Common.h>

namespace networking
{
	class ServerSocket final : public common::NonCopyableObject
	{
	public:
		ServerSocket();
		~ServerSocket();

	public:
		void Close();


	private:
		SOCKET m_oSocket = INVALID_SOCKET;
	};
}