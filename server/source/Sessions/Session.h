#pragma once

#include <Common/Common.h>
#include "Networking/SocketAddress.h"
	 
class Session final
{
public:
	Session(IN int nSessionId, IN const networking::SocketAddress oClientAddress);
	~Session();

public:
	const networking::SocketAddress& GetClientAddress() const;

private:
	int m_nSessionId = 0;
	const networking::SocketAddress m_oClientAddress;
};