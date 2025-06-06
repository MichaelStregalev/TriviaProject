#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "Responses.h"
#include "RoomData.h"
#include "Codes.h"

// CONSTRUCTOR
MenuRequestHandler::MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handler) : m_user(user), m_handlerFactory(handler)
{
}

// IS THE REQUEST RELEVANT?
bool MenuRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	// If the request is any kind of request we can handle, return true
	return	request.requestId >= LOGOUT_REQUEST_CODE && request.requestId <= GET_STATISTICS_REQUEST_CODE;
}

// HANDLE THE REQUEST
RequestResult MenuRequestHandler::handleRequest(const RequestInfo& request)
{
	// The result of the request..
	RequestResult result;

	switch (request.requestId)
	{
	case LOGOUT_REQUEST_CODE:
		result = signout(request);
		break;

	case GET_ROOMS_REQUEST_CODE:
		result = getRooms(request);
		break;

	case GET_PLAYERS_IN_ROOMS_REQUEST_CODE:
		result = getPlayersInRoom(request);
		break;

	case JOIN_ROOM_REQUEST_CODE:
		result = joinRoom(request);
		break;

	case CREATE_ROOM_REQUEST_CODE:
		result = createRoom(request);
		break;

	case GET_HIGHSCORE_REQUEST_CODE:
		result = getHighscore(request);
		break;

	case GET_STATISTICS_REQUEST_CODE:
		result = getStatistics(request);
		break;

	default:	// Shouldn't happen, as we check before if the request is relevant.
		break;
	}

	return result;
}

LoggedUser MenuRequestHandler::getUser() const
{
	return m_user;
}

RequestResult MenuRequestHandler::signout(const RequestInfo& request)
{
	// The result, will be filled at the end with information
	RequestResult result;

	try
	{
		// If the logout as been done successfully...
		if (m_handlerFactory.getLoginManager().logout(m_user.getUsername()))
		{
			LogoutResponse response{ LOGOUT_RESPONSE_CODE };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = m_handlerFactory.createLoginRequestHandler();
			// The new handler will be the login request handler, since the user logged out.
		}
		else
		{
			// Building the unsuccessful signup response
			ErrorResponse response{ "User " + m_user.getUsername() + " does not exist!" };
			result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
			result.newHandler = this;	// The new handler will be a menu request handler, once again since an error occurred!
		}
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

RequestResult MenuRequestHandler::getRooms(const RequestInfo& request)
{
	// The result, will be filled at the end with information
	RequestResult result;
	
	// Use the deserializer in order to get the info from the request
	GetPlayersInRoomRequest getPlayersInRoomRequest = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(request.buffer);

	try
	{
		std::vector<RoomData> rooms = m_handlerFactory.getRoomManager().getRooms();

		// Building the successful signup response
		GetRoomsResponse response{ GET_ROOMS_RESPONSE_CODE };
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

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& request)
{
	RequestResult result;

	// Use the deserializer in order to get the info from the request
	GetPlayersInRoomRequest getPlayersInRoomRequest = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(request.buffer);

	try
	{
		// Getting the Room based on the roomID
		Room* room = m_handlerFactory.getRoomManager().getRoom(getPlayersInRoomRequest.roomId);

		// Getting the vector of all the users (usernames) in the room
		std::vector<std::string> allUsersInRoom;

		// Pushing back only the usernames
		for (const LoggedUser& user : room->getAllUsers())
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

RequestResult MenuRequestHandler::getStatistics(const RequestInfo& request)
{
	// The result, will be filled at the end with information
	RequestResult result;

	try
	{
		// Getting the user's statistics
		std::vector<double> statistics = m_handlerFactory.getStatisticManager().getUserStatistics(m_user.getUsername());

		GetStatisticsResponse response{GET_STATISTICS_RESPONSE_CODE, statistics};
		// Building the successful signup response
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

RequestResult MenuRequestHandler::getHighscore(const RequestInfo& request)
{
	// The result, will be filled at the end with information
	RequestResult result;

	try
	{
		// Getting the high scores
		std::map<std::string, int> highScores = m_handlerFactory.getStatisticManager().getHighScore();

		// Inputting all high scores into a ordered vector:

		// Convert map to vector of pairs, in order for us to have an easier way to sort the scores
		std::vector<std::pair<std::string, int>> scoreVectorPairs(highScores.begin(), highScores.end());

		// Sort by score descending
		std::sort(scoreVectorPairs.begin(), scoreVectorPairs.end(), [](const auto& a, const auto& b) {
			return a.second > b.second;
			});

		// Extract only the scores and names
		std::vector<std::string> names;
		std::vector<int> scores;
		for (const auto& pair : scoreVectorPairs)
		{
			names.push_back(pair.first);
			scores.push_back(pair.second);
		}

		GetHighScoreResponse response{ GET_HIGHSCORE_RESPONSE_CODE, scores, names };
		// Building the successful signup response
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

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& request)
{
	RequestResult result;

	// Use the deserializer in order to get the info from the request
	JoinRoomRequest joinRoomRequest = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(request.buffer);

	try
	{
		// Adding the user to the room the user requested to join
		m_handlerFactory.getRoomManager().getRoom(joinRoomRequest.roomId)->addUser(m_user);

		// Building the response
		JoinRoomResponse response{ JOIN_ROOM_RESPONSE_CODE };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, *(m_handlerFactory.getRoomManager().getRoom(joinRoomRequest.roomId)));
		// The new handler will be RoomMemeberRequestHandler.
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

RequestResult MenuRequestHandler::createRoom(const RequestInfo& request)
{
	RequestResult result;

	// Use the deserializer in order to get the info from the request
	CreateRoomRequest createRoomRequest = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(request.buffer);

	try
	{
		// Getting the room data (the id will be changed when using createRoom)
		RoomData data{ 0, createRoomRequest.roomName, createRoomRequest.maxPlayers, createRoomRequest.questionCount,
						createRoomRequest.answerTimeout, ROOM_OPEN};

		// Creating the room - and getting the id of the room
		int createdRoomId = m_handlerFactory.getRoomManager().createRoom(m_user, data);

		// Building the response
		CreateRoomResponse response{ CREATE_ROOM_RESPONSE_CODE, createdRoomId};
		result.response = JsonResponsePacketSerializer::serializeResponse(response);	// Serializing the response
		result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, *(m_handlerFactory.getRoomManager().getRoom(createdRoomId)));
		// The new handler will be RoomAdminRequestHandler.
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
