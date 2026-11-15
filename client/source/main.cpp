
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <Networking/Packets/ConnectPacket.h>
#include "Application.h"

int main()
{
    Application oGameApplication;
    oGameApplication.Run();

    return 0;
}
