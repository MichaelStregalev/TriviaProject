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
#include "RequestHandlerFactory.h"
#include "IDatabase.h"

#define SERVER_PORT 8876

/*
							COMMUNICATOR
	This class represents the communication aspect of our server,
	it will deal with all of the clients' sockets, requests and responses.
*/

class Communicator
{
public:
	
	// CONSTRUCTOR
	Communicator(RequestHandlerFactory& factory);

	// DECONSTRUCTOR
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
	// Factory handler
	RequestHandlerFactory& m_handlerFactory;

	// <-- PRIVATE METHDOS -->

	void bindAndListen();	// Binding the socket and listening for clients..

	void handleNewClient(SOCKET clientSocket);
	std::string readFromSocket(SOCKET sc, int bytesNum, int flags);						// Reads data from the socket
	RequestInfo messageToRequestInfo(const std::string& binaryMessage) const;			// Turns a message to RequestInfo Struct
	void acceptClient();										// Accepting each client and calling for a thread to handle them

};