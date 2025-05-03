#include "LoginRequestHandler.h"
#include "Codes.h"
#include "Responses.h"
#include "Byte.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	return request.requestId == LOGIN_REQUEST_CODE || request.requestId == SIGNUP_REQUEST_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& request) const
{
	// The result of the request..
	RequestResult result;

	if (request.requestId == LOGIN_REQUEST_CODE)
	{
		// Use the deserializer in order to get the info from the request
		LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);

		// Getting the information from the user in order to check if the login is correct and successful.
		std::string username = loginRequest.userName;
		std::string password = loginRequest.password;

		// here we will check if the login has been successful or not...

		// if successful...
		LoginResponse response{ SUCCESSFUL_LOGIN };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = new LoginRequestHandler();

		// if unsuccessful..
		// ErrorResponse response{"Username or password are not correct!"};
		// result.response = JsonResponsePacketSerializer::serializeResponse(response);
		// result.newHandler = nullptr;
	}
	else if (request.requestId == SIGNUP_REQUEST_CODE)
	{
		// Use the deserializer in order to get the info from the request
		SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(request.buffer);

		// Getting the information from the user in order to check if the login is correct and successful.
		std::string username = signupRequest.userName;
		std::string password = signupRequest.password;
		std::string email = signupRequest.email;

		// here we will check if the login has been successful or not...

		// if successful...
		SignupResponse response{ SUCCESSFUL_SIGNUP };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = new LoginRequestHandler();

		// if unsuccessful..
		// ErrorResponse response{"User already exists with that username!"};
		// result.response = JsonResponsePacketSerializer::serializeResponse(response);
		// result.newHandler = nullptr;
	}
	

	return result;
}
