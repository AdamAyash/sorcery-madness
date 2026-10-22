#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

int main()
{
	WSADATA oWSAData;
	if (WSAStartup(MAKEWORD(2, 2), &oWSAData) != 0)
	{
		return -1;
	}

	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	addrinfo* result;
	if (getaddrinfo("127.0.0.1", "3000", &hints, &result) != 0)
	{
		WSACleanup();
	}

	SOCKET serverSocket = INVALID_SOCKET;
	addrinfo* current = nullptr;
	for (current = result; current != nullptr; current = current->ai_next)
	{
		serverSocket = socket(current->ai_family, current->ai_socktype,
			current->ai_protocol);

		if (serverSocket == INVALID_SOCKET)
		{
			std::cerr << "socket failed with error: %ld\n" << WSAGetLastError() << std::endl;
			WSACleanup();
			return -1;
		}

		bind(serverSocket, current->ai_addr, (int)current->ai_addrlen);

		break;
	}

	while (true)
	{
		char buffer[1024]{};

		int size = sizeof(*current);
		if (!recvfrom(serverSocket, buffer, 1024, 0, current->ai_addr, &size))
		{
			continue;
		}

		std::cout << buffer << std::endl;
	
	}

	return 0;
}