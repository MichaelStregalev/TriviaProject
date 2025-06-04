#pragma once

#include "IRequestHandler.h"
#include "Requests.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR
	RoomAdminRequestHandler(LoggedUser user, Room room, RoomManager& manager, RequestHandlerFactory& handler);
	virtual ~RoomAdminRequestHandler() = default;

	bool isRequestRelevant(const RequestInfo& request) const override;				// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;				// Handle the request

private:

	// <-- FIELDS -->
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// <-- PRIVATE METHODS -->
	RequestResult closeRoom(const RequestInfo& request);
	RequestResult startRoom(const RequestInfo& request);
	RequestResult getRoomState(const RequestInfo& request);
};