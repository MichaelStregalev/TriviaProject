#pragma once

#include <string>
#include "Codes.h"
#include "Question.h"

/*
						GameData
		GameData is a basic structure that will allow us
		to store all data about a game in the server.
*/

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	double averageAnswerTime;
};