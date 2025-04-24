#include <map>
#include <functional> //For std::bind
#include <mutex>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <format>
#include <fstream>
#include <sstream>
#include <condition_variable>
#include "Server.h"

// CONSTRUCTOR
Server::Server()
{

	// this server uses TCP. that why SOCK_STREAM & IPPROTO_TCP
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_serverSocket == INVALID_SOCKET)
	{
		throw std::exception("Failed to create a socket.");
	}
}

// DECONSTRUCTOR
Server::~Server()
{
	try
	{
		closesocket(_serverSocket);
	}
	catch (...)
	{
	}
}

// THE SERVING FUNCTION - WHAT WILL THE SERVER DO WHILE ITS ON
void Server::serve(int port)
{

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Catching exceptions since they are thrown in a detached thread - which is dangerous.
	try
	{
		if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		{
			//throw std::runtime_error("Failed to bind the socket and the configuration.");
			int err = WSAGetLastError();
			std::cerr << "bind() failed, error code: " << err << std::endl;
			throw std::runtime_error("Failed to bind the socket and the configuration.");
		}

		if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			throw std::runtime_error("Failed to listen for incoming requests.");
		}

		std::cout << "Listening on port " << port << std::endl;

		while (true)
		{
			std::cout << "Waiting for client connection request" << std::endl;
			acceptClient();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Thread Exception] Server failed: " << e.what() << std::endl;
	}
}

// ACCEPT CLIENT - WILL MOVE EACH INCOMING CLIENT REQUEST ONTO THE CLIENT HANDLER
void Server::acceptClient()
{
	// this accepts the client and creates a specific socket from server to the incoming client
	// the process will not continue until a client fully connects to the server
	SOCKET client_socket = accept(_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		throw std::exception("The user has failed to connect to the server.");
	}
		
	std::cout << "Client accepted. Server and client can speak" << std::endl;

	// Add the client's socket onto the map of clientSockets
	clientSockets.insert({"", client_socket});
	LoginRequestHandler LRH;

	// Create a thread for the specific client, and handle the client's requests
	std::thread t(&Server::clientHandler, this, client_socket);
	t.detach();	// Detach the thread
}

// CLIENT HANDLER - WILL HANDLE EACH A CLIENT'S REQUESTS
void Server::clientHandler(SOCKET clientSocket)
{
	std::string message = "Hello";

	const char* helloMsg = message.c_str();

	if (send(clientSocket, helloMsg, message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}

	int bytesNum = 5;

	char* data = readFromSocket(clientSocket, bytesNum, 0);
	message = std::string(data);

	std::cout << message << std::endl;

	closesocket(clientSocket);
}

// READ FROM SOCKET - READ DATA FROM THE SOCKET GIVEN
char* Server::readFromSocket(SOCKET sc, int bytesNum, int flags)
{
	if (bytesNum == 0)
	{
		return (char*)"";
	}

	char* data = new char[bytesNum + 1];
	int res = recv(sc, data, bytesNum, flags);

	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}

	data[bytesNum] = 0;
	return data;
}
