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
	if (getaddrinfo("localhost", "3000", &hints, &result) != 0)
	{
		WSACleanup();
	}

	SOCKET clientSocket = INVALID_SOCKET;
	for (addrinfo* current = result; result != nullptr; current = current->ai_next)
	{
		clientSocket = socket(current->ai_family, current->ai_socktype,
			current->ai_protocol);

		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "socket failed with error: %ld\n" << WSAGetLastError() << std::endl;
			WSACleanup();
			return -1;
		}
	}

	return 0;
}