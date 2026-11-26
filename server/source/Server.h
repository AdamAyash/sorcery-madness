#pragma once

#include <Common/Common.h>
#include <Networking/Socket/ServerSocket.h>
#include <Networking/NetworkEndpoint.h>
#include "ServerPacketProcessor/ServerPacketProcessor.h"

class ServerContext;

class Server final : public networking::NetworkEndpoint<networking::ServerSocket>
{
public:
	Server(const std::string &strServerIpAddress, const std::string &strPort);
	~Server();

public:
	bool Start() override;
	bool IsPacketAlreadyQueued(IN const networking::ReceivedPacket &oReceivedPacket) const override;
	void Run();

private:
	bool ProcessPacket(const networking::ReceivedPacket &oReceivedPacket) override;
	void Update();

private:
	bool m_bIsRunning = false;

	std::vector<networking::ReceivedPacket> m_oReceivedPacket;
	common::Scope<ServerContext> m_oServerContext = nullptr;
	common::Scope<ServerPacketProcessor> m_oServerPacketProcessor = nullptr;
};
