#pragma once

#include <string>
#include "Codes.h"

/*
						PlayerResult
		PlayerResults is a basic structure that will allow us
		to store all the results of a player in a game.
*/

struct PlayerResult
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	double averageAnswerTime;
};