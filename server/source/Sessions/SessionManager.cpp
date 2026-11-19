#include "SessionManager.h"
#include "Session.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

SessionManager& SessionManager::GetInstance()
{
	static SessionManager oSessionManager;
	return oSessionManager;
}

int SessionManager::CreateSession(const IN networking::SocketAddress& oClientSocketAddress)
{
	for (auto oClientSessionIterator = m_oClientSessionsMap.begin(); oClientSessionIterator != m_oClientSessionsMap.end(); ++oClientSessionIterator)
	{
		if (oClientSessionIterator->second.GetClientAddress() == oClientSocketAddress)
		{
			LOG_WARN("Client is already connected to the server.");
			return -1;
		}
	}

	Session oSession(m_nNextSessionId, oClientSocketAddress);
	m_oClientSessionsMap.try_emplace(m_nNextSessionId++, oSession);

	return oSession.GetSessionId();
}

const Session& SessionManager::GetSesion(const IN int nSessionId) const
{
	return m_oClientSessionsMap.at(nSessionId);
}