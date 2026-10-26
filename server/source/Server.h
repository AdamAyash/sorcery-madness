#pragma once

#include <Networking/ServerSocket.h>
#include <Networking/WinsockContext.h>
#include <Networking/Packets/RecievedPacket.h>
#include <Common.h>
#include <vector>


class Server final : public common::NonCopyableObject
{
public:
	Server(const std::string& strServerIpAddress, const std::string& strPort);
	~Server();

public:
	bool Start();
	void Run();

private:
	void RecievePackets();
	void ProcessPackets();
	void Update();

private:
	const std::string m_strServerIpAddress;
	const std::string m_strPort;
	bool m_bIsRunning = false;

	common::Scope <networking::WinsockContext> m_oWinsockContext = nullptr;
	common::Scope<networking::ServerSocket> m_oServerSocket = nullptr;

	std::vector<networking:: RecievedPacket> m_oRecivedPackets;
};