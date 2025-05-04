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
#include "Communicator.h"
#include "Responses.h"

// DEFINE
#define MAX_BUFFER_SIZE	1024

Communicator::Communicator(RequestHandlerFactory& factory) : m_handlerFactory(factory)
{
	// Before using Winsock functions like socket(), we must call WSAStartup() exactly once in our program.
	// If it hasn't been done - Winsock won't work and socket() will always return INVALID_SOCKET.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
	}

	// this server uses TCP. that why SOCK_STREAM & IPPROTO_TCP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
	{
		throw std::exception("Failed to create a socket.");
	}
}

Communicator::~Communicator()
{
	// Lock the mutex to safely access the m_clients map
	std::lock_guard<std::mutex> lock(m_clientMutex);

	// Iterate over all the clients
	for (auto& pair : m_clients)
	{
		SOCKET clientSocket = pair.first;
		IRequestHandler* handler = pair.second;

		// Close the client socket
		closesocket(clientSocket);

		// Delete the dynamically allocated handler
		delete handler;
	}

	// Clear the map
	m_clients.clear();

	// Also close the server socket if it's valid
	if (m_serverSocket != INVALID_SOCKET)
	{
		closesocket(m_serverSocket);
	}
}

void Communicator::startHandleRequests()
{
	Communicator::bindAndListen();
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(SERVER_PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Catching exceptions since they are thrown in a detached thread - which is dangerous.
	try
	{
		if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		{
			//throw std::runtime_error("Failed to bind the socket and the configuration.");
			int err = WSAGetLastError();
			std::cerr << "bind() failed, error code: " << err << std::endl;
			throw std::runtime_error("Failed to bind the socket and the configuration.");
		}

		if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			throw std::runtime_error("Failed to listen for incoming requests.");
		}

		std::cout << "Listening on port " << SERVER_PORT << std::endl;

		while (true)
		{
			std::cout << "Waiting for client connection request" << std::endl;
			acceptClient();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Server failed: " << e.what() << std::endl;
	}
}

// ACCEPT CLIENT - WILL MOVE EACH INCOMING CLIENT REQUEST ONTO THE CLIENT HANDLER
void Communicator::acceptClient()
{
	// this accepts the client and creates a specific socket from server to the incoming client
	// the process will not continue until a client fully connects to the server
	SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		throw std::exception("The user has failed to connect to the server.");
	}

	std::cout << "Client accepted. Server and client can speak" << std::endl;

	{
		// Protect the clientSockets map with a lock
		std::lock_guard<std::mutex> lock(m_clientMutex);

		LoginRequestHandler* handler = m_handlerFactory.createLoginRequestHandler();
		// Add the client's socket onto the map of clientSockets
		m_clients.insert({ client_socket, handler });
	}

	// Create a thread for the specific client, and handle the client's requests
	std::thread t(&Communicator::handleNewClient, this, client_socket);
	t.detach();	// Detach the thread
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	while (true)
	{
		try
		{
			//Reading from Socket
			std::string message = readFromSocket(clientSocket, MAX_BUFFER_SIZE, 0);
			RequestInfo requestData = messageToRequestInfo(message);

			if (m_clients[clientSocket]->isRequestRelevant(requestData))
			{
				RequestResult result = m_clients[clientSocket]->handleRequest(requestData);
				std::string requestString = Byte::deserializeBytesToString(requestData.buffer);
				std::cout << requestString << std::endl;
				std::string resultString = Byte::deserializeBytesToString(result.response);
				std::cout << resultString << std::endl;

				if (send(clientSocket, resultString.c_str(), resultString.size(), 0) == INVALID_SOCKET)
				{
					throw std::runtime_error("Error while sending message to client");
				}

				if (m_clients[clientSocket] != result.newHandler)
				{
					delete m_clients[clientSocket];
					m_clients[clientSocket] = result.newHandler;
				}
			}
			else
			{
				throw std::exception("REQUEST NOT RELEVANT");
			}
		}
		catch (const std::runtime_error& e)
		{
			break;
		}
		catch (const std::exception& e)
		{
			// TODO: send the error as a protocol message :)
			send(clientSocket, e.what(), std::strlen(e.what()), 0);
		}
	}
	
	std::cout << "Goodbye :)" << std::endl;
	closesocket(clientSocket);
}

// READ FROM SOCKET - READ DATA FROM THE SOCKET GIVEN
std::string Communicator::readFromSocket(SOCKET sc, int bytesNum, int flags)
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

	data[res] = 0;
	std::string result(data);	// Convert char array to std::string
	delete[] data;				// Don't forget to delete the dynamically allocated memory

	return result;
}

RequestInfo Communicator::messageToRequestInfo(const std::string& binaryMessage) const
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