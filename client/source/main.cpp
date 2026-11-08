
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <Networking/Packets/ConnectPacket.h>
#include "GameApplication.h"

int main()
{
    GameApplication oGameApplication;
    oGameApplication.Run();

    return 0;
}
