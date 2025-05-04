#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:

	MenuRequestHandler() = default;
	virtual ~MenuRequestHandler() = default;


	bool isRequestRelevant(const RequestInfo& request) const override;				// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;			// Handle the request

private:

};
