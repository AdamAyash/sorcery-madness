
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include "GameClient.h"
#include <Networking/Packets/ConnectPacket.h>
#include "GameApplication.h"

int main()
{
    GameApplication oGameApplication;
    oGameApplication.Run();

    /*GameClient oGameClient("127.0.0.1", "3000");
    oGameClient.Start();
  
    networking::ConnectPacket oConnectPacket;
    oConnectPacket.nnRequestId = 1;

    while (true)
    {
        auto bytes = oGameClient.SendPacket(oConnectPacket);
    }*/

    return 0;
}
