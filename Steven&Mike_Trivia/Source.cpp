#pragma comment (lib, "ws2_32.lib")

#include "Server.h"
#include <thread>
#include <iostream>
#include <exception>

// <-- DEFINE CONSTS -->
#define SERVER_PORT 8876

#define SUCCESS 0
#define ERROR 1

/*
This is the main function, it will be responsible for starting the server.
*/

int main()
{
	// Before using Winsock functions like socket(), we must call WSAStartup() exactly once in our program.
	// If it hasn't been done - Winsock won't work and socket() will always return INVALID_SOCKET.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		return ERROR;
	}

	// Trying to dynamically allocate the server
	Server* myServer = nullptr;

	// Now, we can go on building the server and beginning to serve it in a seperate thread
	try
	{
		Server* myServer = new Server();	// Dynamically allocating the server

		std::thread serveThread([&]() { myServer->serve(SERVER_PORT); });
		serveThread.detach();
	}
	catch (const std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
		delete myServer;	// Deallocating the server in case it was allocated.
		system("PAUSE");
		return ERROR;
	}

	std::string statement = "";

	// If the statement 'EXIT' has been entered, exit the program.
	while (statement != "EXIT")
	{
		std::cin >> statement;
	}

	// Deallocating the memory we allocated
	delete myServer;

	system("PAUSE");
	return SUCCESS;
}