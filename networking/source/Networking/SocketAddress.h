#pragma once

#include <Common.h>
#include <winsock2.h>
#include <WS2tcpip.h>

namespace networking
{
	class SocketAddress final
	{
	public:
		SocketAddress(IN const addrinfo& oRawSocketAddress);

		SocketAddress() = default;
		~SocketAddress() = default;

	public:
		const sockaddr& Get() const;
		void Set(IN const sockaddr& oSocketAddress);
		const int Size() const;
		const std::string GetIpAddress() const;

	private:
		sockaddr m_oRawSocketAddress{};
		int m_nSizeInBytes = 0;
	};
}