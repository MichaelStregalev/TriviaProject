#include <map>
#include <functional> //For std::bind
#include <mutex>
#include <iostream>
#include <string>
#include <thread>
#include <format>
#include <fstream>
#include <sstream>
#include "Communicator.h"
#include "Responses.h"
#include "TriviaExceptions.h"
#include "JsonResponsePacketSerializer.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
//#include "GameRequestHandler.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

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
		throw WSASetUpErrorException();
	}

	// this server uses TCP. that why SOCK_STREAM & IPPROTO_TCP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// If an error occurred while creating the socket..
	if (m_serverSocket == INVALID_SOCKET)
	{
		throw SocketCreationErrorException();
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

	WSACleanup();	// Clean the WinSock
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
			throw SocketBindingErrorException();
		}

		if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			throw SocketListeningErrorException();
		}

		std::cout << "Listening on port " << SERVER_PORT << std::endl;

		while (true)
		{
			std::cout << "Waiting for client connection request" << std::endl;
			acceptClient();
		}
	}
	catch (const SocketException& e)
	{
		std::cerr << "Server failed[" << e.what() << "]" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "An error occurred: " << e.what() << std::endl;
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
		throw UserConnectionErrorException();
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
			std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
			// Reading from Socket
			RequestInfo requestData = readRequestInfo(clientSocket);
			std::cout << "REQUEST - CODE:" << requestData.requestId << " MESSAGE: " << Byte::deserializeBytesToString(requestData.buffer) << std::endl;

			if (m_clients[clientSocket]->isRequestRelevant(requestData))
			{
				RequestResult result = m_clients[clientSocket]->handleRequest(requestData);
				char* resultCharArray = Byte::bufferToCharArray(result.response);

				// Getting a string that is full of '\0', later on populating it
				std::string resultString(result.response.size(), '\0');
				for (int i = 0; i < result.response.size(); ++i)
				{
					resultString[i] = result.response[i].asciiChar();
				}

				// Printing the printable and non-printable characters of the result - if non-printable, print the hex values.
				std::cout << "RESULT - ";
				for (size_t i = 0; i < resultString.size(); ++i)
				{
					char c = resultString[i];
					if (std::isprint(static_cast<unsigned char>(c)))
					{
						std::cout << c;
					}
					else
					{
						std::cout << "\\x" << std::hex << std::uppercase << (int)(unsigned char)c << std::dec;
					}
				}

				std::cout << "\n~~~~~~~~~~~~~~~~~" << std::endl;

				if (send(clientSocket, resultString.data(), resultString.size(), 0) == INVALID_SOCKET)
				{
					delete[] resultCharArray;
					throw SendingMessageErrorException();
				}

				delete[] resultCharArray;

				if (m_clients[clientSocket] != result.newHandler)
				{
					delete m_clients[clientSocket];
					m_clients[clientSocket] = result.newHandler;
				}
			}
			else
			{
				throw RequestNotRelevantException();
			}
		}
		catch (const RequestNotRelevantException& e)
		{
			ErrorResponse err;
			err.message = "Non-relevant request.";

			Byte::Buffer bufferOfResponse = JsonResponsePacketSerializer::serializeResponse(err);
			int lengthOfBuffer = Byte::deserializeBytesToString(bufferOfResponse).size();

			char* errCharArray = Byte::bufferToCharArray(bufferOfResponse);

			std::cout << "RESULT ERROR: ";
			for (int i = 0; i < lengthOfBuffer; i++)
			{
				std::cout << errCharArray[i];
			}

			if (send(clientSocket, errCharArray, lengthOfBuffer, 0) == INVALID_SOCKET)
			{
				delete[] errCharArray;
				break;
			}

			delete[] errCharArray;

			continue;
		}
		catch (const SocketException& e)
		{
			// If there has occurred an error with sending a message to the socket - 
			// it means it was closed, and we need to close the socket.

			std::cout << "ERROR EXCEPTION - USER LEFT" << std::endl;

			// In case the user has already logged in, we will sign out for the user.
			IRequestHandler* handler = m_clients[clientSocket];

			if (handler)
			{
				if (MenuRequestHandler* menuHandler = dynamic_cast<MenuRequestHandler*>(handler))
				{
					m_handlerFactory.getLoginManager().logout(menuHandler->getUser().getUsername());
				}

				else if (RoomMemberRequestHandler* roomHandler = dynamic_cast<RoomMemberRequestHandler*>(handler))
				{
					LoggedUser currentUser = roomHandler->getUser();
					int currentRoomId = roomHandler->getRoom().getRoomData().id;

					m_handlerFactory.getLoginManager().logout(currentUser.getUsername());
					m_handlerFactory.getRoomManager().getRoom(currentRoomId)->removeUser(currentUser);
				}

				else if (RoomAdminRequestHandler* roomHandler = dynamic_cast<RoomAdminRequestHandler*>(handler))
				{
					LoggedUser currentUser = roomHandler->getUser();

					m_handlerFactory.getLoginManager().logout(currentUser.getUsername());
					roomHandler->handleRequest(RequestInfo(CLOSE_ROOM_REQUEST_CODE, ""));
				}

				else if (GameRequestHandler* gameHandler = dynamic_cast<GameRequestHandler*>(handler))
				{
					LoggedUser currentUser = gameHandler->getUser();
					gameHandler->getGame().removeUser(currentUser);

					m_handlerFactory.getLoginManager().logout(currentUser.getUsername());
				}
			}

			break;
		}
		catch (const std::exception& e)
		{
			// If an exception had been thrown and no once catched it - 
			// it means an error occurred, we will close the connection.
			std::cout << "ERROR OCCURRED " << e.what() << std::endl;
			break;
		}
	}
	
	// Since the conversation ended here - remove the client from the m_clients, and delete the dynamically allocated handler

	{
		// Protect the clientSockets map with a lock
		std::lock_guard<std::mutex> lock(m_clientMutex);

		// Delete the handler
		delete m_clients[clientSocket];

		// Remove the client from m_clients
		m_clients.erase(clientSocket);
	}


	std::cout << "USER CONNECTION GONE" << std::endl;
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
		throw ReceivingMessageErrorException();
	}

	data[res] = 0;
	std::string result(data);	// Convert char array to std::string
	delete[] data;				// Don't forget to delete the dynamically allocated memory

	std::cout << result << std::endl;

	return result;
}

RequestInfo Communicator::readRequestInfo(SOCKET sc)
{
	char codeInByte;
	int res = recv(sc, &codeInByte, 1, 0);
	if (res == INVALID_SOCKET)
	{
		throw ReceivingMessageErrorException();
	}

	char lengthInByte[4];
	res = recv(sc, lengthInByte, 4, 0);
	if (res == INVALID_SOCKET)
	{
		throw ReceivingMessageErrorException();
	}

	int length = *(int*)lengthInByte;

	char* message = new char[length + 1];
	res = recv(sc, message, length, 0);
	if (res == INVALID_SOCKET)
	{
		throw ReceivingMessageErrorException();
	}

	message[length] = '\0';
	std::string messageString(message);

	delete[] message;

	RequestInfo info((int)codeInByte, messageString);

	return info;
}
