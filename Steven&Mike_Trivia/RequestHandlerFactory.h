#pragma once

#include "LoginManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

class LoginRequestHandler;

/*
						RequestHandlerFactory
	RequestHandlerFactory is a class that will help us centerize all of our classes into
	a single class, letting the design of the project be a lot more cleaner and comfortable.
*/

class RequestHandlerFactory
{
public:
	// CONSTRUCTOR
	RequestHandlerFactory(IDatabase* db);

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();
	LoginManager& getLoginManager();

private:

	LoginManager m_loginManager;
	IDatabase* m_database;

};