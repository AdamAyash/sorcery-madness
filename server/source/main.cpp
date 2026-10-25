#include <iostream>
#include "WinsockContext.h"
#include <ServerSocket.h>

int main()
{
	networking::WinsockContext oWinsockContext;
	if (!oWinsockContext.Initialize())
	{

	}

	networking::ServerSocket oServerSocket("127.0.0.1", "2551");
	if (!oServerSocket.Open())
	{

	}

	while (true)
	{
		char buffer[1024]{};
		oServerSocket.Recieve(buffer);
		std::cout << buffer << std::endl;
	}

	return 0;
}