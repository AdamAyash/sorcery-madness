#pragma once

#include <Common.h>
#include <Networking/SocketAddress.h>
	 
class ClientSession final
{
public:
	ClientSession(IN int nPlayerId, IN const networking::SocketAddress oClientAddress);
	~ClientSession();

private:
	int m_nPlayerId = 0;
	const networking::SocketAddress m_oClientAddress;
};