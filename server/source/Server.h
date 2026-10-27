#pragma once

#include <Common.h>
#include <Networking/ServerSocket.h>
#include <Networking/Packets/RecievedPacket.h>
#include <Networking/NetworkEndpoint.h>
#include "ServerContext.h"


class Server final : public networking::NetworkEndpoint<networking::ServerSocket>
{
public:
	Server(const std::string& strServerIpAddress, const std::string& strPort);
	~Server();

public:
	bool Start() override;
	void Run();

private:
	bool ProcessPacket(const networking::RecievedPacket& oRecievedPacket) override;
	void Update();

private:
	bool m_bIsRunning = false;
	std::vector<networking:: RecievedPacket> m_oRecivedPackets;
	common::Scope<ServerContext> m_oServerContext = nullptr;
};