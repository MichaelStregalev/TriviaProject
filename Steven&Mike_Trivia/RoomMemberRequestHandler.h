#pragma once

#include "IRequestHandler.h"
#include "Requests.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR
	RoomMemberRequestHandler(const LoggedUser& user, int roomId, RoomManager& manager, RequestHandlerFactory& handler);
	virtual ~RoomMemberRequestHandler() = default;

	bool isRequestRelevant(const RequestInfo& request) const override;				// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;				// Handle the request

	// GETTER
	LoggedUser getUser() const;
	Room getRoom() const;

private:

	// <-- FIELDS -->
	int m_roomId;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// <-- PRIVATE METHODS -->
	RequestResult leaveRoom();
	RequestResult getRoomState();
	RequestResult startGame();
	RequestResult getPlayersInRoom();		// Added so we can get the players in the room
};