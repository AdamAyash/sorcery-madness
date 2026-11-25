#include "SessionManager.h"
#include "Session.h"
#include <Networking/NetworkingGlobals.h>

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

std::optional<int> SessionManager::CreateSession(const IN networking::SocketAddress& oClientSocketAddress)
{
	if(m_oClientSessionsMap.size() >= networking::n_MAX_PLAYER_SESSIONS_SUPPORTED)
		return std::nullopt;

	for (auto oClientSessionIterator = m_oClientSessionsMap.begin(); oClientSessionIterator != m_oClientSessionsMap.end(); ++oClientSessionIterator)
	{
		if (oClientSessionIterator->second.GetClientAddress() == oClientSocketAddress)
		{
			LOG_WARN("Client is already connected to the server.");
			return std::nullopt;
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