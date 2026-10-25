#pragma once

#include <Common.h>
#include <string>
#include <winsock2.h>
#include "SocketAddress.h"

namespace networking
{
	class BaseSocket : public common::NonCopyableObject
	{
	protected:
		BaseSocket(IN const std::string& strDomainAddress, IN const std::string& strPort);

	public:
		virtual ~BaseSocket();

	public:
		virtual bool Open();
		int Send(IN const char* pMessageBuffer, IN const int nMessageBufferSizeInBytes) const;
		int Recieve(OUT char* pMessageBuffer, OUT SocketAddress* pSenderAddress = nullptr) const;
		void Close();

	protected:
		const std::string m_strDomainAddress;
		const std::string m_strPort;

		SocketAddress m_oSocketAddress;
		SOCKET m_oSocket = INVALID_SOCKET;
	};
}