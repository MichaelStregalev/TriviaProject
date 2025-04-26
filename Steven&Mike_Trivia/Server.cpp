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

// DEFINE
#define MAX_BUFFER_SIZE	1024

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

	// Protect the clientSockets map with a lock
	std::lock_guard<std::mutex> lock(_clientMutex);

	// Add the client's socket onto the map of clientSockets
	_clientSockets.insert({"", client_socket});
	LoginRequestHandler LRH;

	// Create a thread for the specific client, and handle the client's requests
	std::thread t(&Server::clientHandler, this, client_socket);
	t.detach();	// Detach the thread
}

// CLIENT HANDLER - WILL HANDLE EACH A CLIENT'S REQUESTS
void Server::clientHandler(SOCKET clientSocket)
{
	std::string message = readFromSocket(clientSocket, MAX_BUFFER_SIZE, 0);
	std::cout << message << std::endl;

	RequestInfo requestData = messageToRequestInfo(message);
	std::cout << "Goodbye :)" << std::endl;
	closesocket(clientSocket);
}

// READ FROM SOCKET - READ DATA FROM THE SOCKET GIVEN
std::string Server::readFromSocket(SOCKET sc, int bytesNum, int flags)
{
	if (bytesNum == 0)
	{
		return "";
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
	std::string result(data);	// Convert char array to std::string
	delete[] data;				// Don't forget to delete the dynamically allocated memory

	return result;
}

RequestInfo Server::messageToRequestInfo(const std::string& binaryMessage) const
{
	RequestInfo result;
	
	// Getting the arrival time
	time_t timeStamp;
	time(&timeStamp);
	result.receivalTime = timeStamp; // Inserting into Struct

	Byte::Buffer buffer = Byte::separateBinary(binaryMessage);
	
	// Getting the Request ID
	Byte codeRequest(buffer[REQUEST_ID_IN_BUFFER]);
	result.requestId = codeRequest.decimalValue(); // Inserting into Struct

	std::string dataLengthBin; // This will be a binary string representing the data length
	int i = 0;
	for (i = DATA_LENGTH_START; i <= DATA_LENGTH_END; i++)
	{
		dataLengthBin += buffer[i].binaryCode();
	}

	int dataLength = Byte::calculateDecimalValue(dataLengthBin); // Getting the decimal data length

	// Getting the message itself into the buffer
	Byte::Buffer message;
	for (int i = DATA_START; i < DATA_START + dataLength; i++)
	{
		message.push_back(buffer[i]);
	}

	result.buffer = message; //Inserting into the struct the message buffer (which only contains the JSON data)

	return result;
}