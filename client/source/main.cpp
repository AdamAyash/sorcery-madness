
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <Networking/Packets/ConnectPacket.h>
#include "Game.h"
#include "GameApplication.h"

int main()
{
   /* Game oGameApplication;
    oGameApplication.Run();*/

    GameApplication gameApp;
    gameApp.Run();

    return 0;
}
