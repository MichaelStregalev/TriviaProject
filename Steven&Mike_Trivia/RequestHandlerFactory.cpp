#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(LoginManager lm, IDatabase* db)
	:m_loginManager(db), m_database(db)
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return nullptr;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	LoginManager result(m_database);
	
	return result;
}
