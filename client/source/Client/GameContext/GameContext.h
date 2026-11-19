#pragma once

#include <Common/Common.h>
#include "../ClientConnectionStatuses.h"
#include <mat4x4.hpp>

class GameContext final : public common::NonCopyableObject
{
public:
	GameContext();
	~GameContext();

public:
	int GetSessionId() const;
	void SetSessionId(IN const int nSessionId);

	bool IsClientStatusConnected() const;
	void SetClientStatusAsConnecting();
	void SetClientStatusAsConnected();

private:
	ClientConnectionStatuses m_eClientConnectionStatus = ClientConnectionStatuses::Disconnected;
	int m_nSessionId = 0;

public:
	std::unordered_map<int, glm::vec3> oPlayersMap;
};