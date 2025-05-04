#pragma once

#include <thread>
#include <iostream>
#include <functional>
#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"

// <-- DEFINE CONSTS -->
#define SUCCESS 0
#define ERROR_CODE 1

/*
							SERVER
	This class represents our server for the Trivia Project.
	This class is responsible for running the server, and using the
	communicator in order to handle the clients.
	Our server is a MultiThread server, it can include multiple
	users at the same time, and handle each of them seperately.
*/

class Communicator;

class Server
{
public:

	// CONSTRUCTOR
	Server(IDatabase* db);

	// SERVING METHOD - HOW THE SERVER WORKS
	void run();

private:

	// <-- FIELDS -->
	Communicator m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;
};