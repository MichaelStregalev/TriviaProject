#include "RoomMemberRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, const Room& room, RoomManager& manager, RequestHandlerFactory& handler) :
    m_user(user), m_room(room), m_roomManager(manager), m_handlerFactory(handler)
{

}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return request.requestId == LEAVE_ROOM_REQUEST_CODE || request.requestId == GET_ROOM_STATE_REQUEST_CODE 
								|| request.requestId == START_GAME_REQUEST_CODE;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& request)
{
	// The result of the request..
	RequestResult result;

	switch (request.requestId)
	{
	case LEAVE_ROOM_REQUEST_CODE:
		result = leaveRoom(request);
		break;

	case GET_ROOM_STATE_REQUEST_CODE:
		result = getRoomState(request);
		break;

	case START_GAME_REQUEST_CODE:
		result = startGame(request);
		break;

	default:	// Shouldn't happen, as we check before if the request is relevant.
		break;
	}

	return result;
}

LoggedUser RoomMemberRequestHandler::getUser() const
{
	return m_user;
}

Room RoomMemberRequestHandler::getRoom() const
{
	return m_room;
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& request)
{
	RequestResult result;

	try
	{
		// Try and leave the room..
		m_room.removeUser(m_user);
		LeaveRoomResponse response{ LEAVE_ROOM_RESPONSE_CODE };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
		// If you leave the room - the new handler will be once again the menu request handler
	}
	catch (const std::exception& e)
	{
		// Building the unsuccessful signup response
		ErrorResponse response{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = this;	// The new handler will be the same, as an error occurred.
	}

	return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& request)
{
	RequestResult result;

	try
	{
		
		GetRoomStateResponse response{ GET_ROOM_STATE_RESPONSE_CODE };
		response.hasGameBegun = m_room.getRoomData().roomStatus == GAME_STARTED;
		response.answerTimeout = m_room.getRoomData().timePerQuestion;
		response.questionCount = m_room.getRoomData().numOfQuestionsInGame;
		
		std::vector<std::string> usernames;	// Getting all of the usernames for response.players
		for (const auto& user : m_room.getAllUsers())
		{
			usernames.push_back(user.getUsername());
		}
		response.players = usernames;

		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = this; // The new handler will remain the same
	}
	catch (const std::exception& e)
	{
		// Building the unsuccessful signup response
		ErrorResponse response{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = this;	// The new handler will be the same, as an error occurred.
	}

	return result;
}

RequestResult RoomMemberRequestHandler::startGame(const RequestInfo& request)
{
	RequestResult result;

	try
	{
		StartGameResponse response{ START_GAME_RESPONSE_CODE };
		
		// This function will be called in case the admin already started the game and the client of the rest
		// of the users has noticed hasGameBegun == true!!
		// So, this function will be the same as the startGame in the RoomAdminRequestHandler but - won't change the roomdata.

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);	// needs to change to game request handler!!
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}
