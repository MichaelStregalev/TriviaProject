#include "RoomAdminRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const LoggedUser& user, Room& room, RoomManager& manager, RequestHandlerFactory& handler) :
    m_user(user), m_room(room), m_roomManager(manager), m_handlerFactory(handler)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return request.requestId == CLOSE_ROOM_REQUEST_CODE || request.requestId == START_GAME_REQUEST_CODE 
		|| request.requestId == GET_ROOM_STATE_REQUEST_CODE || request.requestId == GET_PLAYERS_IN_ROOMS_REQUEST_CODE;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& request)
{
	// The result of the request..
	RequestResult result;

	switch (request.requestId)
	{
	case CLOSE_ROOM_REQUEST_CODE:
		result = closeRoom();
		break;

	case START_GAME_REQUEST_CODE:
		result = startGame();
		break;

	case GET_ROOM_STATE_REQUEST_CODE:
		result = getRoomState();
		break;

	case GET_PLAYERS_IN_ROOMS_REQUEST_CODE:
		result = getPlayersInRoom();
		break;

	default:	// Shouldn't happen, as we check before if the request is relevant.
		break;
	}

	return result;
}

LoggedUser RoomAdminRequestHandler::getUser() const
{
	return m_user;
}

Room RoomAdminRequestHandler::getRoom() const
{
	return m_room;
}

RequestResult RoomAdminRequestHandler::closeRoom()
{
	RequestResult result;

	try
	{
		CloseRoomResponse response{ CLOSE_ROOM_RESPONSE_CODE };

		// Remove the admin from the and room remove the room from the manager
		m_room.removeUser(m_user);
		m_roomManager.deleteRoom(m_room.getRoomData().id);

		// Set current user back to Menu page
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}

RequestResult RoomAdminRequestHandler::startGame()
{
	RequestResult result;

	try
	{
		StartGameResponse response{ START_GAME_RESPONSE_CODE };

		// Setting the rooms status to GAME_STARTED
		m_room.getRoomData().roomStatus = GAME_STARTED;

		// Getting the new Game
		Game& newGame = m_handlerFactory.getGameManager().createGame(m_room, m_room.getRoomData().numOfQuestionsInGame);

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, newGame);
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}

RequestResult RoomAdminRequestHandler::getRoomState()
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

RequestResult RoomAdminRequestHandler::getPlayersInRoom()
{
	RequestResult result;

	try
	{
		// Getting the vector of all the users (usernames) in the room
		std::vector<std::string> allUsersInRoom;

		// Pushing back only the usernames
		for (const LoggedUser& user : m_room.getAllUsers())
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
