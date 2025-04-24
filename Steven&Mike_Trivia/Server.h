#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <string>
#include "LoginRequestHandler.h"
#include "IRequestHandler.h"

/*
							SERVER
	This class represents our server for the Trivia Project.
	This class includes accepting clients and handling all of their
	requests up till they finish!
	Our server is a MultiThread server, it can include multiple
	users at the same time, and handle each of them seperately.
*/

class Server
{
public:

	// CONSTRUCTOR
	Server();

	// DECONSTRUCTOR
	~Server();

	// SERVING METHOD - HOW THE SERVER WORKS
	void serve(int port);

private:

	// <-- FIELDS -->

	// The server socket
	SOCKET _serverSocket;
	// The map of all client sockets in the server
	std::map<std::string, SOCKET> clientSockets;
	
	// <-- PRIVATE METHODS -->

	void acceptClient();									// Makes sure the client is accepted (fully connected to the server)
	void clientHandler(SOCKET clientSocket);				// Handles the client, its requests and anything else that the client needs.
	char* readFromSocket(SOCKET sc, int bytesNum, int flags);	// Reads data from the socket
};