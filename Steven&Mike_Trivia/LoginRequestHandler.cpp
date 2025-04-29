#include "LoginRequestHandler.h"
#include "Codes.h"
#include "Responses.h"
#include "Byte.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "SqliteDatabase.h"
#include "LoginManager.h"

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& request)
{
	return request.requestId == LOGIN_REQUEST_CODE || request.requestId == SIGNUP_REQUEST_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& request)
{
	RequestResult result;
	SqliteDatabase db;
	LoginManager lm(&db);
	db.open();

	// The result of the request..

	if (request.requestId == LOGIN_REQUEST_CODE)
	{
		// Use the deserializer in order to get the info from the request
		LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);

		// Getting the information from the user in order to check if the login is correct and successful.
		std::string username = loginRequest.userName;
		std::string password = loginRequest.password;

		// here we will check if the login has been successful or not...
		if (!(lm.login(username, password)))
		{
			// if successful...
			LoginResponse response{ SUCCESSFUL_LOGIN };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = new LoginRequestHandler();
		}
		else
		{
			//if unsuccessful..
			ErrorResponse response{ "Username or password are not correct!" };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = nullptr;
		}

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
		//If 0 is returned, SignUp was succesfull
		if (!(lm.signup(username, password, email)))
		{
			// if successful...
			SignupResponse response{ SUCCESSFUL_SIGNUP };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = new LoginRequestHandler();
		}
		else
		{
			// if unsuccessful..
			ErrorResponse response{ "User already exists with that username!" };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = nullptr;
		}

	}

	db.close();
	return result;
}
