#include "RoomAdminRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser user, Room room, RoomManager& manager, RequestHandlerFactory& handler) :
    m_user(user), m_room(room), m_roomManager(manager), m_handlerFactory(handler)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return request.requestId == CLOSE_ROOM_REQUEST_CODE || request.requestId == START_GAME_REQUEST_CODE ||
                            request.requestId == GET_ROOM_STATE_REQUEST_CODE;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& request)
{
	// The result of the request..
	RequestResult result;

	switch (request.requestId)
	{
	case CLOSE_ROOM_REQUEST_CODE:
		result = closeRoom(request);
		break;

	case START_GAME_REQUEST_CODE:
		result = startGame(request);
		break;

	case GET_ROOM_STATE_REQUEST_CODE:
		result = getRoomState(request);
		break;

	default:	// Shouldn't happen, as we check before if the request is relevant.
		break;
	}

	return result;
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& request)
{
	RequestResult result;
	try
	{
		// Get all users in the room
		auto users = m_room.getAllUsers();

		// Create the response to send to all users
		LeaveRoomResponse response{ LEAVE_ROOM_RESPONSE_CODE };

		for (const auto& user : users)
		{
			//??
		}

		// Remove the room from the manager
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

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& request)
{
	RequestResult result;

	try
	{
		StartGameResponse response{ START_GAME_RESPONSE_CODE };
		auto buffer = JsonResponsePacketSerializer::serializeResponse(response);

		auto users = m_room.getAllUsers();

		for (const auto& user : users)
		{
			//??
		}

		// Setting the rooms status to ROOM_STARTED
		m_room.getRoomData().roomStatus = ROOM_STARTED;

		result.response = buffer;
		//result.newHandler = m_handlerFactory.createGameRequestHandler(??);
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& request)
{
	RequestResult result;

	try
	{
		GetRoomStateResponse response{ GET_ROOM_STATE_RESPONSE_CODE };
		response.hasGameBegun = m_room.getRoomData().roomStatus == ROOM_STARTED;
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
