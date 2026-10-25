#include <iostream>
#include "Server.h"

int main()
{
	Server oServer("127.0.0.1", "3000");

	if(!oServer.Start())
	{
		LOG_ERROR("Failed to start the server.");
		return -1;
	}

	oServer.Run();

	return 0;
}