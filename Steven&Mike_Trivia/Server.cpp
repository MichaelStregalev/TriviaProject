#include "Server.h"

// THE SERVING FUNCTION - WHAT WILL THE SERVER DO WHILE ITS ON
void Server::run()
{
	// Before using Winsock functions like socket(), we must call WSAStartup() exactly once in our program.
	// If it hasn't been done - Winsock won't work and socket() will always return INVALID_SOCKET.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
	}

	std::thread t_connecter([this]() 
		{
		m_communicator.startHandleRequests();
		});

	t_connecter.detach();

	std::string statement = "";

	// If the statement 'EXIT' has been entered, exit the program.
	while (statement != "EXIT")
	{
		std::cin >> statement;
	}

	system("PAUSE");
}

