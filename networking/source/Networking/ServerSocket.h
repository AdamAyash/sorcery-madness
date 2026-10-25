#pragma once

#include "BaseSocket.h"

namespace networking
{
	class ServerSocket final : public BaseSocket
	{
	public:
		ServerSocket(IN const std::string& strDomainAddress, IN const std::string& strPort);
		virtual ~ServerSocket();

	public:
		bool Open() override;

	private:
		bool Bind();
	};
}