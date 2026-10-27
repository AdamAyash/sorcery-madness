#include "ClientSession.h"

ClientSession::ClientSession(IN int nPlayerId, IN const networking::SocketAddress oClientAddress)
	: m_nPlayerId(nPlayerId)
	, m_oClientAddress(oClientAddress)
{

}

ClientSession::~ClientSession()
{

}