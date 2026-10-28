#include "ClientSession.h"

ClientSession::ClientSession(IN int nPlayerId, IN const networking::SocketAddress oClientAddress)
	: m_nPlayerId(nPlayerId)
	, m_oClientAddress(oClientAddress)
{
}

ClientSession::~ClientSession()
{
}

const networking::SocketAddress& ClientSession::GetClientAddress() const
{
	return m_oClientAddress;
}