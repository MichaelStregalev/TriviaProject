#pragma once

#include "IRequestHandler.h"
#include "Byte.h"
#include <ctime>
#include <string>

class IRequestHandler;

/*
				RESPONSES
	This header includes all of the
	different types of requests a user can make,
	and their structures.
*/

struct RequestInfo
{
	int requestId;
	time_t receivalTime;
	Byte::Buffer buffer;
};

struct RequestResult
{
	Byte::Buffer response;
	IRequestHandler* newHandler = nullptr;
};

struct LoginRequest
{
	std::string userName;
	std::string password;
};

struct SignupRequest
{
	std::string userName;
	std::string password;
	std::string email;
};