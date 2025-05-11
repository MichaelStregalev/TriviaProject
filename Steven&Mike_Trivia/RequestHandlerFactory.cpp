#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db)
	:m_loginManager(LoginManager(db)), m_database(db), m_statisticsManager(StatisticManager(db)), m_roomManager(RoomManager())
{
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return m_roomManager;
}

StatisticManager& RequestHandlerFactory::getStatisticManager()
{
	return m_statisticsManager;
}


LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
	return new MenuRequestHandler();
}