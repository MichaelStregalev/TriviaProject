#pragma once

#include "IRequestHandler.h"
#include "Requests.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR
	RoomAdminRequestHandler(const LoggedUser& user, const Room& room, RoomManager& manager, RequestHandlerFactory& handler);
	virtual ~RoomAdminRequestHandler() = default;

	bool isRequestRelevant(const RequestInfo& request) const override;				// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;				// Handle the request

	// GETTER
	LoggedUser getUser() const;
	Room getRoom() const;

private:

	// <-- FIELDS -->
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// <-- PRIVATE METHODS -->
	RequestResult closeRoom(const RequestInfo& request);
	RequestResult startGame(const RequestInfo& request);
	RequestResult getRoomState(const RequestInfo& request);
};