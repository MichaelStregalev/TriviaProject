#pragma once

#include "Byte.h"
#include "json.hpp"
#include "Responses.h"
#include "Codes.h"

// DEFINES

#define ERROR_MESSAGE_FIELD		"message"
#define STATUS_FIELD			"status"

/*
					JsonResponsePacketSerializer
	This namespace will allow us to serialize any type of responses onto
	binary code, and send them to our users!
*/
namespace JsonResponsePacketSerializer
{
	Byte::Buffer serializeResponse(const ErrorResponse& response);
	Byte::Buffer serializeResponse(const SignupResponse& response);
	Byte::Buffer serializeResponse(const LoginResponse& response);

	// HELPER METHOD - BASE SERIALIZATION
	Byte::Buffer baseSerialization(int code, const std::string& data);
}