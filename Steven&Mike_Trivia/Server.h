#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <functional> //For std::bind
#include <queue>
#include <mutex>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <format>
#include <fstream>
#include <condition_variable>
#include <sstream>
#include <map>
#include "LoginRequestHandler.h"
#include "IRequestHandler.h"

class Server
{
public:
	Server();
	~Server();
	void serve(int port);

private:

	void acceptClient();
	void clientHandler(SOCKET clientSocket);
	char* readFromSocket(SOCKET sc, int bytesNum, int flags);

	SOCKET _serverSocket;
};

