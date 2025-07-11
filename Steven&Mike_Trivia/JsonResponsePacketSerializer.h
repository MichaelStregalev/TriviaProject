#pragma once

#include "Byte.h"
#include "json.hpp"
#include "Responses.h"
#include "Codes.h"

// <-- DEFINE CONSTS FOR FIELDS

#define		ERROR_MESSAGE_FIELD			"message"
#define		STATUS_FIELD				"status"
#define		ROOMS_FIELD					"rooms"
#define		PLAYERS_FIELD				"players"
#define		STATISTICS_FIELD			"statistics"
#define		SCORES_FIELD				"scores"
#define		NAMES_FIELD					"names"
#define		GAME_BEGUN_FIELD			"hasGameBegun"
#define		PLAYERS_FIELD				"players"
#define		QUESTION_COUNT_FIELD		"questionCount"
#define		ANSWER_TIMEOUT_FIELD		"answerTimeout"
#define		ROOM_ID_FIELD				"roomId"
#define		CORRECT_ANSWER_ID_FIELD		"correctAnswerId"
#define		RESULTS_FIELD				"results"
#define		QUESTION_FIELD				"question"
#define		ANSWERS_FIELD				"answers"

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

	Byte::Buffer serializeResponse(const LogoutResponse& response);
	Byte::Buffer serializeResponse(const GetRoomsResponse& response);
	Byte::Buffer serializeResponse(const GetPlayersInRoomResponse& response);
	Byte::Buffer serializeResponse(const JoinRoomResponse& response);
	Byte::Buffer serializeResponse(const CreateRoomResponse& response);
	Byte::Buffer serializeResponse(const GetHighScoreResponse& response);
	Byte::Buffer serializeResponse(const GetStatisticsResponse& response);

	Byte::Buffer serializeResponse(const CloseRoomResponse& response);
	Byte::Buffer serializeResponse(const StartGameResponse& response);
	Byte::Buffer serializeResponse(const GetRoomStateResponse& response);
	Byte::Buffer serializeResponse(const LeaveRoomResponse& response);

	Byte::Buffer serializeResponse(const GetGameResultsResponse& response);
	Byte::Buffer serializeResponse(const SubmitAnswerResponse& response);
	Byte::Buffer serializeResponse(const GetQuestionResponse& response);
	Byte::Buffer serializeResponse(const LeaveGameResponse& response);

	// <-- HELPER METHODS -->

	// Base serialization - will do the serialization of the data, given the data and the code for the serialization
	Byte::Buffer baseSerialization(int code, const std::string& data);

	// Convert RoomData to JSON
	nlohmann::json roomDataToJson(const RoomData& room);
	// Convert PlayerResult to JSON
	nlohmann::json playerResultToJson(const PlayerResult& result);
}