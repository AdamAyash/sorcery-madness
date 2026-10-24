
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <cstring>
#include "WinsockContext.h"

int main()
{
    networking::WinsockContext oWinsockContext;
    if (!oWinsockContext.Initialize())
    {

    }

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    addrinfo* result = nullptr;

    if (getaddrinfo("127.0.0.1", "3000", &hints, &result) != 0)
    {
        std::cerr << "getaddrinfo failed\n";
        WSACleanup();
        return -1;
    }

    SOCKET clientSocket = INVALID_SOCKET;
    addrinfo* current = nullptr;

    for (current = result;
        current != nullptr;
        current = current->ai_next)
    {
        clientSocket = socket(
            current->ai_family,
            current->ai_socktype,
            current->ai_protocol
        );

        if (clientSocket != INVALID_SOCKET)
        {
            break;
        }
    }

    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "socket failed: "
            << WSAGetLastError() << '\n';

        freeaddrinfo(result);
        WSACleanup();
        return -1;
    }

    const char* message = "Message";

    while (true)
    {
        int sent = sendto(
            clientSocket,
            message,
            static_cast<int>(strlen(message)),
            0,
            current->ai_addr,
            static_cast<int>(current->ai_addrlen)
        );

        if (sent == SOCKET_ERROR)
        {
            std::cerr << "sendto failed: "
                << WSAGetLastError() << '\n';
            break;
        }

        std::cout << "Sent " << sent << " bytes\n";
    }

    closesocket(clientSocket);
    freeaddrinfo(result);

    return 0;
}
