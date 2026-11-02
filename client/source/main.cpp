
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include "GameClient.h"
#include <Networking/Packets/ConnectPacket.h>
#include <Engine/Engine.h>

int main()
{
    common::Logger::Init("Client");

    engine::Engine& oEngine = engine::Engine::GetInstance();
    if (!oEngine.Initialize())
    {
        return -1;
    }

    oEngine.Run();

    GameClient oGameClient("127.0.0.1", "3000");
    oGameClient.Start();
  
    networking::ConnectPacket oConnectPacket;
    oConnectPacket.nnRequestId = 1;

    while (true)
    {
        auto bytes = oGameClient.SendPacket(oConnectPacket);
    }

    return 0;
}
