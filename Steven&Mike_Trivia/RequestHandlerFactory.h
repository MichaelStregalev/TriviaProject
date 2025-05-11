#pragma once

#include "LoginManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "StatisticManager.h"
#include "RoomManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

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

	// <-- METHODS -->
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	RoomAdminRequestHandler* createRoomAdminRequestHandler();
	RoomMemberRequestHandler* createRoomMemberRequestHandler();

	
private:

	// <-- FIELDS -->
	LoginManager m_loginManager;
	IDatabase* m_database;
	RoomManager m_roomManager;
	StatisticManager m_statisticsManager;


};