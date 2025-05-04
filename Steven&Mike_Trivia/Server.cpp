#include "Server.h"

Server::Server(IDatabase* db) : m_database(db), m_handlerFactory(RequestHandlerFactory(db)), m_communicator(m_handlerFactory)
{
}

// THE SERVING FUNCTION - WHAT WILL THE SERVER DO WHILE ITS ON
void Server::run()
{
	std::thread t_connecter([this]() 
		{
		m_communicator.startHandleRequests();
		});

	t_connecter.detach();

	std::string statement = "";

	// If the statement 'EXIT' has been entered, exit the program.
	while (statement != "EXIT")
	{
		std::cin >> statement;
	}

	system("PAUSE");
}

