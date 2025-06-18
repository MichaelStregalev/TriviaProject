#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db)
	:m_loginManager(LoginManager(db)), m_database(db), m_statisticsManager(StatisticManager(db)), m_roomManager(RoomManager()), m_gameManager(GameManager(db, m_roomManager))
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

GameManager& RequestHandlerFactory::getGameManager()
{
	return m_gameManager;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
	return new MenuRequestHandler(user, *this);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, Room& room)
{
	return new RoomAdminRequestHandler(user, room, m_roomManager, *this);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, Room& room)
{
	return new RoomMemberRequestHandler(user, room.getRoomData().id, m_roomManager, *this);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user, Game& game)
{
	return new GameRequestHandler(game, user, m_gameManager, *this);
}
