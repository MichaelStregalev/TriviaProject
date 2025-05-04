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

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& request) const
{
	// The result of the request..
	RequestResult result;

	if (request.requestId == LOGIN_REQUEST_CODE)
	{
		result = login(request);
	}
	else if (request.requestId == SIGNUP_REQUEST_CODE)
	{
		result = signup(request);
	}

	return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& request) const
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
			result.newHandler = new MenuRequestHandler();		// The new handler will be the menu request handler!
		}
		else
		{
			// Building the unsuccessful signup response
			ErrorResponse response { "User with the username " + signupRequest.userName + " already exists!"};
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = new LoginRequestHandler(m_handlerFactory);	// The new handler will be a login request handler, once again since an error occurred!
		}
	}
	catch (const std::exception& e)
	{
		// Building the unsuccessful signup response
		ErrorResponse response{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = new LoginRequestHandler(m_handlerFactory);	// The new handler will be a login request handler, once again since an error occurred!
	}


	return result;
}

RequestResult LoginRequestHandler::login(const RequestInfo& request) const
{
	RequestResult result;

	// Use the deserializer in order to get the info from the request
	LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);

	// Getting the information from the user and trying to login

	// If the login was successful..
	try
	{
		if (m_handlerFactory.getLoginManager().login(loginRequest.userName, loginRequest.password))
		{
			// Building the successful login response
			LoginResponse response{ SUCCESSFUL_LOGIN };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = new MenuRequestHandler();		// The new handler will be the menu request handler!
		}
		else
		{
			// Building the unsuccessful login response
			ErrorResponse response{ "Login was unsuccessful! Check that username and password are correct, or that the user isn't already connected." };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = new LoginRequestHandler(m_handlerFactory);	// The new handler will be a login request handler, once again since an error occurred!
		}
	}
	catch (const std::exception& e)
	{
		// Building the unsuccessful login response
		ErrorResponse response{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = new LoginRequestHandler(m_handlerFactory);	// The new handler will be a login request handler, once again since an error occurred!
	}
	
	return result;
}
