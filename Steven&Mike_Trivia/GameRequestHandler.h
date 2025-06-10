#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

/*
						GameRequestHandler
	GameRequestHandler is a child class of IRequestHandler,
	it represents the handler of all that is related to the game system.
*/

class GameRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR & DECONSTRUCTOR
	GameRequestHandler(RequestHandlerFactory& handler);
	virtual ~GameRequestHandler() = default;

	// METHODS

	bool isRequestRelevant(const RequestInfo& request) const override;			// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;			// Handle the request!

private:

	// <-- FIELDS -->
	RequestHandlerFactory& m_handlerFactory;

};
