#include "GameContext.h"

GameContext::GameContext()
{
}

GameContext::~GameContext()
{
}

bool GameContext::IsClientStatusConnected() const
{
	return m_eClientConnectionStatus == ClientConnectionStatuses::Connected;
}

void GameContext::SetClientStatusAsConnecting()
{
	m_eClientConnectionStatus = ClientConnectionStatuses::Connecting;
}

void GameContext::SetClientStatusAsConnected()
{
	m_eClientConnectionStatus = ClientConnectionStatuses::Connected;
}

int GameContext::GetSessionId() const
{
	return m_nSessionId;
}

void GameContext::SetSessionId(IN const int nSessionId)
{
	m_nSessionId = nSessionId;
}