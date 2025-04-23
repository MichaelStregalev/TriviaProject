#pragma comment (lib, "ws2_32.lib")

#include "Server.h"
#include <iostream>
#include <exception>

int main()
{
	try
	{
		Server myServer;

		std::thread srv(&Server::serve, myServer, 8876);
		srv.detach();

	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}

	bool flag = true;
	std::string statment;

	while (flag)
	{
		std::cin >> statment;

		if (statment == "EXIT")
		{
			flag = false;
		}
	}

	system("PAUSE");
	return 0;
}