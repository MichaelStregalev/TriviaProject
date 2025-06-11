#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "StatisticManager.h"
#include "RoomManager.h"
#include "GameManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;

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

	// GETTERS
	LoginManager& getLoginManager();
	RoomManager& getRoomManager();
	StatisticManager& getStatisticManager();
	GameManager& getGameManager();

	// <-- METHODS -->
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);

	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, Room& room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, Room& room);

	GameRequestHandler* createGameRequestHandler(const LoggedUser& user, Game& game);

private:

	// <-- FIELDS -->
	LoginManager m_loginManager;
	IDatabase* m_database;
	RoomManager m_roomManager;
	StatisticManager m_statisticsManager;
	GameManager m_gameManager;


};