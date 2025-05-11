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
} typedef LoginResponse;

struct SignupResponse
{
	unsigned int status;
} typedef SignupResponse;

struct ErrorResponse
{
	std::string message;
} typedef ErrorResponse;

struct LogoutResponse
{
	unsigned int status;
} typedef LogoutResponse;

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
} typedef GetRoomsResponse;

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
} typedef GetPlayersInRoomResponse;

struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<int> scores;
} typedef GetHighScoreResponse;

struct GetStatisticsResponse
{
	unsigned int status;
	std::vector<double> statistics;
} typedef GetStatisticsResponse;

struct JoinRoomResponse
{
	unsigned int status;
} typedef JoinRoomResponse;

struct CreateRoomResponse
{
	unsigned int status;
} typedef CreateRoomResponse;