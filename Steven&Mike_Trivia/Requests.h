#pragma once

#include "IRequestHandler.h"
#include "Byte.h"
#include <ctime>

struct RequestInfo
{
	int RequestId;
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