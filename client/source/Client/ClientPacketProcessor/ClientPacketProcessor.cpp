#include "ClientPacketProcessor.h"
#include "../GameContext/GameContext.h"
#include <Networking/Networking.h>
#include <Networking/Packets/UpdateWorldStatePacket.h>
#include <Networking/Packets/ConnectionEstablishedPacket.h>

ClientPacketProcessor::ClientPacketProcessor(IN GameContext& oGameContext)
	: m_oGameContext(oGameContext)
{
}

ClientPacketProcessor::~ClientPacketProcessor()
{
}

bool ClientPacketProcessor::ProcessPacket(const networking::RecievedPacket& oRecievedPacket)
{
	switch (oRecievedPacket.ePacketType)
	{
	case networking::PacketTypes::ConnectionEstablished:
		return ProcessConnectionEstablishedRequest(oRecievedPacket);

	case networking::PacketTypes::UpdateWorldState:
		return ProcessUpdateWorldStateRequest(oRecievedPacket);

	default:
		LOG_ERROR("Unsupported packet type: ", static_cast<int>(oRecievedPacket.ePacketType));
		return false;
	}

	return true;
}

bool ClientPacketProcessor::ProcessConnectionEstablishedRequest(IN const networking::RecievedPacket& oRecievedPacket)
{
	const auto oConnectionEstablishedPacket = networking::DeserializePacket<networking::ConnectionEstablishedPacket>(oRecievedPacket);
	m_oGameContext.SetSessionId(oConnectionEstablishedPacket->nPlayerSessionId);
	m_oGameContext.SetClientStatusAsConnected();

	return true;
}

bool ClientPacketProcessor::ProcessUpdateWorldStateRequest(IN const networking::RecievedPacket& oRecievedPacket)
{
	const auto oUpdateWorldStatePacket = networking::DeserializePacket<networking::UpdateWorldStatePacket>(oRecievedPacket);

	const auto oPlayerStateDataElementSize = sizeof(oUpdateWorldStatePacket->playerStateDataArray) / sizeof(networking::PlayerStateData);
	const auto oPlayerStateDataArray = std::vector<networking::PlayerStateData>(oUpdateWorldStatePacket->playerStateDataArray, 
		oUpdateWorldStatePacket->playerStateDataArray + oPlayerStateDataElementSize);

	auto& oPlayersMap = m_oGameContext.oPlayersMap;

	for (const auto& oPlayerStateDataIterator : oPlayerStateDataArray)
	{
		const auto nCurrentSessionId = oPlayerStateDataIterator.nSessionId;

		if(nCurrentSessionId <= 0)
			continue;

		if (!oPlayersMap.contains(nCurrentSessionId))
		{
			oPlayersMap.emplace(nCurrentSessionId, glm::vec3(oPlayerStateDataIterator.positionX, oPlayerStateDataIterator.positionY, 0));
			continue;
		}

		oPlayersMap[nCurrentSessionId] = glm::vec3(oPlayerStateDataIterator.positionX, oPlayerStateDataIterator.positionY, 0);
	}

	return true;
}