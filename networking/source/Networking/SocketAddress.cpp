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

	const std::string SocketAddress::GetIpAddress() const
	{
		char szIpAddress[INET6_ADDRSTRLEN]{};

		const auto* ipv4Address = reinterpret_cast<const sockaddr_in*>(&m_oRawSocketAddress);

		if (inet_ntop(AF_INET, &ipv4Address->sin_addr, szIpAddress, sizeof(szIpAddress)) == nullptr)
			return std::string();

		return std::string(szIpAddress);
	}
}