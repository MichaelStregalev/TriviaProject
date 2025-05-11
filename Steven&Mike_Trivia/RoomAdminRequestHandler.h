#pragma once

#include "IRequestHandler.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR
	RoomAdminRequestHandler() = default;
	virtual ~RoomAdminRequestHandler() = default;

	bool isRequestRelevant(const RequestInfo& request) const override;				// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;				// Handle the request
};