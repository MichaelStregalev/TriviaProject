#pragma once

#include <string>
#include "Codes.h"

/*
						RoomData
		RoomData is a basic structure that will allow us
		to store all data about a room in the server.
*/

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	enum RoomStatus roomStatus;
};