
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <cstring>

#include "WinsockContext.h"
#include "Socket.h"

int main()
{
    common::Logger::Init();

    LOG_CLIENT_INFO("Test message");

    networking::WinsockContext oWinsockContext;
    if (!oWinsockContext.Initialize())
    {

    }

    networking::Socket oClientSocket("127.0.0.1", "2551");
    if (!oClientSocket.Open())
    {

    }

    const char* message = "Message";

    while (true)
    {
        auto bytes = oClientSocket.Send(message, 7);
        //std::cout << "Sent " << bytes << " bytes\n";
    }

    return 0;
}
