#pragma once

#include <Networking/NetworkEndpoint.h>
#include <Networking/Socket/Socket.h>
#include "ClientConnectionStatuses.h"

struct networking::RecievedPacket;

class GameClient final : public networking::NetworkEndpoint<networking::Socket>
{
public:
	GameClient(IN const std::string& strServerIpAddress, IN const std::string& strPort);
	virtual ~GameClient();

public:
	bool Start() override;
	bool IsPacketAlreadyQueued(IN const networking::RecievedPacket& oRecievedPacket) const override;
	bool ConnectToServer();
	void Update();

	const ClientConnectionStatuses GetClientConnectionStatus() const;

private:
	bool ProcessPacket(const networking::RecievedPacket& oRecievedPacket) override;

private:
	bool m_bIsRunning = false;
	ClientConnectionStatuses m_eClientConnectionStatus = ClientConnectionStatuses::Disconnected;

	std::vector<networking::RecievedPacket> m_oRecivedPackets;
};