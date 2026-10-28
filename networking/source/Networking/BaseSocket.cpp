#include "BaseSocket.h"
#include "Networking.h"

namespace networking
{
	BaseSocket::BaseSocket(IN const std::string& strDomainAddress, IN const std::string& strPort)
		: m_strDomainAddress(strDomainAddress)
		, m_strPort(strPort)
	{
	}

	BaseSocket:: ~BaseSocket()
	{
		Close();
	}

	bool BaseSocket::Open()
	{
		addrinfo oAddressOptions{};
		oAddressOptions.ai_family = AF_INET;
		oAddressOptions.ai_socktype = SOCK_DGRAM;
		oAddressOptions.ai_protocol = IPPROTO_UDP;

		addrinfo* pAddressResult = nullptr;

		if (0 != getaddrinfo(m_strDomainAddress.c_str(), m_strPort.c_str(), &oAddressOptions, &pAddressResult))
		{
			//TODO ERROR
			return false;
		}

		addrinfo* pAddressIterator = nullptr;
		for (pAddressIterator = pAddressResult; pAddressIterator != nullptr; pAddressIterator = pAddressIterator->ai_next)
		{
			m_oSocket = socket(pAddressIterator->ai_family, pAddressIterator->ai_socktype, pAddressIterator->ai_protocol);
			if (INVALID_SOCKET != m_oSocket)
			{
				u_long ulMode = 1;
				if (SOCKET_ERROR == ioctlsocket(m_oSocket, FIONBIO, &ulMode))
				{
					//TODO ERROR
					continue;
				}

				m_oSocketAddress = SocketAddress(*pAddressIterator);
				break;
			}
		}

		freeaddrinfo(pAddressResult);
		return true;
	}

	int BaseSocket::Send(IN const char* pMessageBuffer, IN const int nMessageBufferSizeInBytes) const
	{
		if (m_oSocket == INVALID_SOCKET)
		{
			LOG_ERROR("Closed or corrupted socket.");
			return SOCKET_ERROR;
		}

		if (nMessageBufferSizeInBytes <= 0)
		{
			//TODO ERROR
			return SOCKET_ERROR;
		}

		auto nBytesSent = sendto(
			m_oSocket,
			pMessageBuffer,
			nMessageBufferSizeInBytes,
			0,
			&m_oSocketAddress.Get(),
			m_oSocketAddress.Size()
		);

		return nBytesSent;
	}

	int BaseSocket::Recieve(OUT char* pMessageBuffer, OUT SocketAddress* pSenderAddress /*= nullptr*/) const
	{
		if (m_oSocket == INVALID_SOCKET)
		{
			LOG_ERROR("Closed or corrupted socket.");
			return SOCKET_ERROR;
		}

		sockaddr oAddressFrom {};
		int nAddressFromSizeInBytes = sizeof(oAddressFrom);

		const int bytesRecieved = recvfrom(m_oSocket, 
			pMessageBuffer, 
			n_MAX_PACKET_SIZE, 
			0, 
			&oAddressFrom, 
			&nAddressFromSizeInBytes);

		if (pSenderAddress)
			pSenderAddress->Set(oAddressFrom);

		return bytesRecieved;
	}

	void BaseSocket::Close()
	{
		if (m_oSocket != INVALID_SOCKET)
		{
			closesocket(m_oSocket);
			m_oSocket = INVALID_SOCKET;
		}
	}
}