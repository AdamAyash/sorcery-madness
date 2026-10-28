#pragma once

#include <Networking/NetworkEndpoint.h>
#include <Networking/Socket.h>

class GameClient final : public networking::NetworkEndpoint<networking::Socket>
{
public:
	GameClient(const std::string& strServerIpAddress, const std::string& strPort);
	~GameClient();

public:
	bool Start() override;
	void Run();

private:
	bool ProcessPacket(const networking::RecievedPacket& oRecievedPacket) override;
	void Update();

private:
	bool m_bIsRunning = false;
	std::vector<networking::RecievedPacket> m_oRecivedPackets;
};