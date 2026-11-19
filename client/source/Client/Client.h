#pragma once

#include <Networking/NetworkEndpoint.h>
#include <Networking/Socket/Socket.h>
#include "ClientPacketProcessor/ClientPacketProcessor.h"

class ClientContext;
struct networking::RecievedPacket;

class Client final : public networking::NetworkEndpoint<networking::Socket>
{
public:
	Client(IN const std::string& strServerIpAddress, IN const std::string& strPort, IN GameContext& oGameContext);
	virtual ~Client();

public:
	bool Start() override;
	bool IsPacketAlreadyQueued(IN const networking::RecievedPacket& oRecievedPacket) const override;
	bool ConnectToServer();
	void Update();

private:

private:
	bool ProcessPacket(const networking::RecievedPacket& oRecievedPacket) override;

private:
	bool m_bIsRunning = false;
	GameContext& m_oGameContext;
	common::Scope<ClientPacketProcessor> m_oClientPacketProcessor;
	std::vector<networking::RecievedPacket> m_oRecivedPackets;
};