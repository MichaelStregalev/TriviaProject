#include "RoomMemberRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"
#include "TriviaExceptions.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, int roomId, RoomManager& manager, RequestHandlerFactory& handler) :
    m_user(user), m_roomId(roomId), m_roomManager(manager), m_handlerFactory(handler)
{

}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return request.requestId == LEAVE_ROOM_REQUEST_CODE || request.requestId == GET_ROOM_STATE_REQUEST_CODE 
		|| request.requestId == START_GAME_REQUEST_CODE || request.requestId == GET_PLAYERS_IN_ROOMS_REQUEST_CODE;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& request)
{
	// The result of the request..
	RequestResult result;

	switch (request.requestId)
	{
	case LEAVE_ROOM_REQUEST_CODE:
		result = leaveRoom();
		break;

	case GET_ROOM_STATE_REQUEST_CODE:
		result = getRoomState();
		break;

	case START_GAME_REQUEST_CODE:
		result = startGame();
		break;

	case GET_PLAYERS_IN_ROOMS_REQUEST_CODE:
		result = getPlayersInRoom();
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
	Room* room = m_roomManager.getRoom(m_roomId);

	if (room)
	{
		return *room;
	}

	throw RoomDoesNotExistException(m_roomId);
}

RequestResult RoomMemberRequestHandler::leaveRoom()
{
	RequestResult result;

	try
	{
		// In the case of leaveRoom, we want to know whether the room exists or not, since in some cases -
		// the user will want to leave a room that has been deleted. in that case, we just want to change the handler.
		// getRoom throws an exception in case a room does not exist...
		try
		{
			Room* room = m_roomManager.getRoom(m_roomId);

			(*room).removeUser(m_user);
		}
		catch (const std::exception& e)
		{
			// If the room does not exist or has been deleted... do nothing :)
		}
		
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

RequestResult RoomMemberRequestHandler::getRoomState()
{
	RequestResult result;

	try
	{
		Room* room = m_roomManager.getRoom(m_roomId);

		GetRoomStateResponse response{ GET_ROOM_STATE_RESPONSE_CODE };
		response.hasGameBegun = (*room).getRoomData().roomStatus == GAME_STARTED;
		response.answerTimeout = (*room).getRoomData().timePerQuestion;
		response.questionCount = (*room).getRoomData().numOfQuestionsInGame;
		
		std::vector<std::string> usernames;	// Getting all of the usernames for response.players
		for (const auto& user : (*room).getAllUsers())
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

RequestResult RoomMemberRequestHandler::startGame()
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

RequestResult RoomMemberRequestHandler::getPlayersInRoom()
{
	RequestResult result;

	try
	{
		Room* room = m_roomManager.getRoom(m_roomId);

		// Getting the vector of all the users (usernames) in the room
		std::vector<std::string> allUsersInRoom;

		// Pushing back only the usernames
		for (const LoggedUser& user : (*room).getAllUsers())
		{
			allUsersInRoom.push_back(user.getUsername());
		}

		// Building the response
		GetPlayersInRoomResponse response{ allUsersInRoom };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = this;
		// The new handler will still be the menu request handler.
	}
	catch (const std::exception& e)
	{
		// Building the unsuccessful signup response
		ErrorResponse response{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = this;	// The new handler will be a menu request handler, once again since an error occurred!
	}

	return result;
}
