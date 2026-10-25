#include "SocketAddress.h"

namespace networking
{
	SocketAddress::SocketAddress(IN const addrinfo& oRawSocketAddress)
	{
		m_oRawSocketAddress = *oRawSocketAddress.ai_addr;
		m_nSizeInBytes = static_cast<int>(oRawSocketAddress.ai_addrlen);
	}

	const sockaddr& SocketAddress::Get() const
	{
		return m_oRawSocketAddress;
	}

	void SocketAddress::Set(IN const sockaddr& oSocketAddress)
	{
		m_oRawSocketAddress = oSocketAddress;
		m_nSizeInBytes = static_cast<int>(sizeof(oSocketAddress));
	}

	const int SocketAddress::Size() const
	{
		return m_nSizeInBytes;
	}
}