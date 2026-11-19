#include "ServerContext.h"
#include <Networking/Networking.h>
#include <Common/Common.h>
#include "../Sessions/Session.h"

ServerContext::ServerContext()
{
}

ServerContext::~ServerContext()
{
}

void ServerContext::AddPlayer(IN const networking::PlayerStateData& oPlayerStateData)
{
	m_oPlayersStateDataList.push_back(oPlayerStateData);
}

const networking::PlayersStateDataList& ServerContext::GetPlayerStateDataList() const
{
	return m_oPlayersStateDataList;
}