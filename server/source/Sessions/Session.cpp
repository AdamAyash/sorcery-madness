#include "Session.h"

Session::Session(IN int nPlayerId, IN const networking::SocketAddress oClientAddress)
	: m_nSessionId(nPlayerId)
	, m_oClientAddress(oClientAddress)
{
}

Session::~Session()
{
}

const networking::SocketAddress& Session::GetClientAddress() const
{
	return m_oClientAddress;
}