#pragma once

#include "Requests.h"

/*
						IRequestHandler
	IRequestHandler is an abstract class that represents each and all of the
	different handlers for each type of reuqest a user can make.
*/

struct RequestInfo;
struct RequestResult;

class IRequestHandler
{
public:

	// CONSTRUCTOR & DECONSTRUCTOR
	IRequestHandler() = default;
	~IRequestHandler() = default;

	// METHODS

	virtual bool isRequestRelevant(const RequestInfo& request) const = 0;			// Is the request relevant?
	virtual RequestResult handleRequest(const RequestInfo& request) = 0;			// Handle the request

	virtual void userLeftUnexpectedly() = 0;										// User left unexceptedly!
};