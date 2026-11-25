#pragma once

#include <Common/Common.h>
#include <Networking/SocketAddress.h>

class Session;

class SessionManager final : public common::NonCopyableObject
{
private:
	SessionManager();
public:
	~SessionManager();

public:
	static SessionManager& GetInstance();
	std::optional<int> CreateSession(const IN networking::SocketAddress& oClientSocketAddress);
	const Session& GetSesion(const IN int nSessionId) const;

private:
	std::unordered_map<int, Session> m_oClientSessionsMap;
	int m_nNextSessionId = 1;
};