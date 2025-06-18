#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"

class RequestHandlerFactory;

/*
						MenuRequestHandler
	MenuRequestHandler is a child class of IRequestHandler,
	it represents the handler of all the options of actions a user
	can do when already logged in.
*/

class MenuRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR
	MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handler);
	virtual ~MenuRequestHandler() = default;

	bool isRequestRelevant(const RequestInfo& request) const override;				// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;				// Handle the request

	void userLeftUnexpectedly() override;											// The user left unexpectedly

private:

	// <-- FIELDS -->

	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	// <-- HELPER METHODS FOR HANDLING REQUESTS -->

	RequestResult signout(const RequestInfo& request);
	RequestResult getRooms(const RequestInfo& request);
	RequestResult getPlayersInRoom(const RequestInfo& request);
	RequestResult getStatistics(const RequestInfo& request);
	RequestResult getHighscore(const RequestInfo& request);
	RequestResult joinRoom(const RequestInfo& request);
	RequestResult createRoom(const RequestInfo& request);
};