#pragma once
#include "IRequestHandler.h"

/*
						LoginRequestHandler
	LoginRequestHandler is a child class of IRequestHandler,
	it represents the handler of the LoginRequest a user can make.
*/

class LoginRequestHandler : public IRequestHandler
{
public:
	
	// CONSTRUCTOR & DECONSTRUCTOR
	LoginRequestHandler() = default;
	~LoginRequestHandler() = default;

	// METHODS

	virtual bool isRequestRelevant(const RequestInfo& request) const override;			// Is the request relevant?
	virtual RequestResult handleRequest(const RequestInfo& request) const override;		// Handle the request!

private:

};
