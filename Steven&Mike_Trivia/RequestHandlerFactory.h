#pragma once

#include "LoginManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"

class RequestHandlerFactory
{
public:

	RequestHandlerFactory(LoginManager lm, IDatabase* db);
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();

private:

	LoginManager m_loginManager;
	IDatabase* m_database;

};