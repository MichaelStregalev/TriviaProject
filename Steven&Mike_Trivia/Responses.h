#pragma once

#include <string>

/*
				RESPONSES
	This header includes all of the
	different types of response structures.
*/

struct LoginResponse
{
	unsigned int status;
} typedef LoginResponse;

struct SignupResponse
{
	unsigned int status;
} typedef SignupResponse;

struct ErrorResponse
{
	std::string message;
} typedef ErrorResponse;