#pragma once

#include <Common/Common.h>
#include <Networking/SocketAddress.h>
	 
class ClientSession final
{
public:
	ClientSession(IN int nPlayerId, IN const networking::SocketAddress oClientAddress);
	~ClientSession();

public:
	const networking::SocketAddress& GetClientAddress() const;

private:
	int m_nPlayerId = 0;
	const networking::SocketAddress m_oClientAddress;
};