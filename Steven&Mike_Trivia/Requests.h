#pragma once

#include "Byte.h"
#include <ctime>
#include <string>

class IRequestHandler;

/*
					REQUESTS
	This header includes all of the
	different types of requests a user can make,
	and their structures.
*/

struct RequestInfo
{
	int requestId;
	time_t receivalTime;
	Byte::Buffer buffer;

	RequestInfo(int requestId, std::string message)
	{
		this->requestId = requestId;

		this->buffer = Byte::stringToBuffer(message);

		time_t timeStamp;
		time(&timeStamp);
		this->receivalTime = timeStamp; // Inserting into Struct
	}
};

struct RequestResult
{
	Byte::Buffer response;
	IRequestHandler* newHandler = nullptr;
} typedef RequestResult;

// <-- LOGIN MANAGER REQUESTS -->

struct LoginRequest
{
	std::string userName;
	std::string password;
} typedef LoginRequest;

struct SignupRequest
{
	std::string userName;
	std::string password;
	std::string email;
} typedef SignupRequest;

// <-- ROOM REQUESTS -->

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
} typedef GetPlayersInRoomRequest;

struct JoinRoomRequest
{
	unsigned int roomId;
} typedef JoinRoomRequest;

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxPlayers;
	unsigned int questionCount;
	unsigned int answerTimeout;
} typedef CreateRoomRequest;