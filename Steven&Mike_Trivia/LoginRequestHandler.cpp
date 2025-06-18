#include "LoginRequestHandler.h"
#include "Codes.h"
#include "Responses.h"
#include "Byte.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handler) : m_handlerFactory(handler)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	return request.requestId == LOGIN_REQUEST_CODE || request.requestId == SIGNUP_REQUEST_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& request)
{
	// The result of the request..
	RequestResult result;

	switch (request.requestId)
	{
	case LOGIN_REQUEST_CODE:
		result = login(request);
		break;

	case SIGNUP_REQUEST_CODE:
		result = signup(request);
		break;

	default:	// Shouldn't happen, as we check before if the request is relevant.
		break;
	}

	return result;
}

// We won't do anything if the user unexpectedly leaves the Login/Signup
void LoginRequestHandler::userLeftUnexpectedly()
{
}

RequestResult LoginRequestHandler::signup(const RequestInfo& request)
{
	RequestResult result;

	// Use the deserializer in order to get the info from the request
	SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(request.buffer);

	// Getting the information from the user and trying to signup

	// If the signup was successful..
	try
	{
		if (m_handlerFactory.getLoginManager().signup(signupRequest.userName, signupRequest.password, signupRequest.email))
		{
			// Building the successful signup response
			SignupResponse response { SUCCESSFUL_SIGNUP };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = m_handlerFactory.createMenuRequestHandler(LoggedUser(signupRequest.userName));		
			// The new handler will be the menu request handler!
		}
		else
		{
			// Building the unsuccessful signup response
			ErrorResponse response { "User with the username " + signupRequest.userName + " already exists!"};
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = this;	// The new handler will be a login request handler, once again since an error occurred!
		}
	}
	catch (const std::exception& e)
	{
		// Building the unsuccessful signup response
		ErrorResponse response{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = this;	// The new handler will be a login request handler, once again since an error occurred!
	}


	return result;
}

RequestResult LoginRequestHandler::login(const RequestInfo& request)
{
	RequestResult result;

	std::cout << "LOGIN REQUEST HANDELR: LOGIN" << std::endl;

	// Use the deserializer in order to get the info from the request
	LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);

	// Getting the information from the user and trying to login

	// If the login was successful..
	try
	{
		// in case an exception is thrown - the user does not exist.
		int codeOfLogin = m_handlerFactory.getLoginManager().login(loginRequest.userName, loginRequest.password);

		if (codeOfLogin == LOGIN_SUCCESSFUL)
		{
			// Building the successful login response
			LoginResponse response{ SUCCESSFUL_LOGIN };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = m_handlerFactory.createMenuRequestHandler(LoggedUser(loginRequest.userName));		
			// The new handler will be the menu request handler!
		}
		else if (codeOfLogin == PASSWORD_DONT_MATCH)
		{
			// Building the unsuccessful login response - if login returned PASSWORD_DONT_MATCH - passwords do not match.
			ErrorResponse response{ "The password does not match the user." };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = this;	// The new handler will be a login request handler, once again since an error occurred!
		}
		else
		{
			// Building the unsuccessful login response - if login returned anything else (ALREADY_CONNECTED) - 
			// the user is already connected to the server.
			ErrorResponse response{ "The user is already connected to the server." };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = this;	// The new handler will be a login request handler, once again since an error occurred!
		}
	}
	catch (const std::exception& e)
	{
		// Building the unsuccessful login response
		ErrorResponse response{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = this;			// The new handler will be a login request handler, once again since an error occurred!
	}
	
	return result;
}
