#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

/*
						LoginRequestHandler
	LoginRequestHandler is a child class of IRequestHandler,
	it represents the handler of the LoginRequest a user can make.
*/

class LoginRequestHandler : public IRequestHandler
{
public:
	
	// CONSTRUCTOR & DECONSTRUCTOR
	LoginRequestHandler(RequestHandlerFactory& handler);
	virtual ~LoginRequestHandler() = default;

	// METHODS

	bool isRequestRelevant(const RequestInfo& request) const override;			// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;			// Handle the request!

private:

	// <-- FIELDS -->
	RequestHandlerFactory& m_handlerFactory;

	// <-- PRIVATE METHODS -->
	RequestResult signup(const RequestInfo& request);
	RequestResult login(const RequestInfo& request);

};
