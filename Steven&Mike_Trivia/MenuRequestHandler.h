#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:

	MenuRequestHandler() = default;
	virtual ~MenuRequestHandler() = default;


	virtual bool isRequestRelevant(const RequestInfo& request) const;				// Is the request relevant?
	virtual RequestResult handleRequest(const RequestInfo& request) const;			// Handle the request

private:

};
