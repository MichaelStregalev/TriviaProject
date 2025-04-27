#pragma once

#include <WinSock2.h>
#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <string>
#include <mutex>
#include "LoginRequestHandler.h"
#include "IRequestHandler.h"
#include "Codes.h"

#define SERVER_PORT 8876

/*
							COMMUNICATOR
	This class represents the communication aspect of our server,
	it will deal with all of the clients' sockets, requests and responses.
*/

class Communicator
{
public:
	
	// No need to write a constructor if we don't need any important initializations.
	// But - we do need to write the deconstructor as we have dynamically allocated some information.
	~Communicator();

	void startHandleRequests();	// This function will let us start handling requests from clients!

private:

	// <-- FIELDS -->
	
	// The server socket
	SOCKET m_serverSocket;
	// The map of all client sockets in the server
	std::map<SOCKET, IRequestHandler*> m_clients;
	// The mutex of clientSockets
	std::mutex m_clientMutex;

	// <-- PRIVATE METHDOS -->

	void bindAndListen();	// Binding the socket and listening for clients..

	void handleNewClient(SOCKET clientSocket);
	std::string readFromSocket(SOCKET sc, int bytesNum, int flags);						// Reads data from the socket
	RequestInfo messageToRequestInfo(const std::string& binaryMessage) const;			// Turns a message to RequestInfo Struct
	void acceptClient();										// Accepting each client and calling for a thread to handle them

};