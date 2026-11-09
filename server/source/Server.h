#pragma once

#include <Common/Common.h>
#include <Networking/Socket/ServerSocket.h>
#include <Networking/NetworkEndpoint.h>
#include "ServerContext.h"

struct networking::RecievedPacket;

class Server final : public networking::NetworkEndpoint<networking::ServerSocket>
{
public:
	Server(const std::string& strServerIpAddress, const std::string& strPort);
	~Server();

public:
	bool Start() override;
	bool IsPacketAlreadyQueued(IN const networking::RecievedPacket& oRecievedPacket) const override;
	void Run();

private:
	bool ProcessPacket(const networking::RecievedPacket& oRecievedPacket) override;
	void Update();

private:
	bool m_bIsRunning = false;
	std::vector<networking:: RecievedPacket> m_oRecivedPackets;
	common::Scope<ServerContext> m_oServerContext = nullptr;
};