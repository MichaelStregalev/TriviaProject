#pragma once

#include "IRequestHandler.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR
	RoomMemberRequestHandler() = default;
	virtual ~RoomMemberRequestHandler() = default;

	bool isRequestRelevant(const RequestInfo& request) const override;				// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;				// Handle the request
};