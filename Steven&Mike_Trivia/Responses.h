#pragma once

#include <string>
#include <vector>
#include "RoomData.h"

/*
				RESPONSES
	This header includes all of the
	different types of response structures.
*/

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct ErrorResponse
{
	std::string message;
};

struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};

struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<int> scores;
};

struct GetStatisticsResponse
{
	unsigned int status;
	std::vector<double> statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
};