
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <cstring>

#include "Networking/WinsockContext.h"
#include "Networking/Socket.h"
#include <Networking/Packets/PacketHeader.h>

int main()
{
    common::Logger::Init();

    networking::WinsockContext oWinsockContext;
    if (!oWinsockContext.Initialize())
    {

    }

    networking::Socket oClientSocket("127.0.0.1", "3000");
    if (!oClientSocket.Open())
    {

    }

    networking::PacketHeader oPacketHeader;
    oPacketHeader.ePacketType = networking::PacketTypes::PacketTypeConnect;

    const char* message = reinterpret_cast<const char*>(&oPacketHeader);

    while (true)
    {
        auto bytes = oClientSocket.Send(message, 16);
        //std::cout << "Sent " << bytes << " bytes\n";
    }

    return 0;
}
