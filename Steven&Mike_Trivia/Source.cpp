#pragma comment (lib, "ws2_32.lib")

#include "Server.h"
#include <iostream>

/*
This is the main function, it will be responsible for starting the server.
*/

int main()
{
	Server server;
	server.run();

	return 0;
}