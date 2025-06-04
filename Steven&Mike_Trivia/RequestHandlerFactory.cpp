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

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
	return new MenuRequestHandler(user, *this);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room room)
{
	return new RoomAdminRequestHandler(user, room, m_roomManager, *this);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room room)
{
	return new RoomMemberRequestHandler(user, room, m_roomManager, *this);
}