#pragma once

#include <vector>
#include <map>
#include "Question.h"
#include "LoggedUser.h"
#include "GameData.h"
#include "Room.h"
#include "PlayerResult.h"

/*
									GAME
			This class represents a single game in our trivia server,
	it will let us handle a single game - like submit answers of users, and such.
*/

class Game
{
public:

	// EMPTY CONSTRUCTOR
	Game();

	// CONSTRUCTOR
	Game(const Room& room, std::vector<Question> questions);

	// GETTERS
	Question getCurrentQuestionForUser(const LoggedUser& user);
	std::vector<PlayerResult> getGameResults();


	// METHODS
	void getNextQuestionForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, unsigned int answerId, double answerTime);
	void removeUser(const LoggedUser& user);

	bool gameFinished();
	bool moreQuestionsForUser(const LoggedUser& user);

private:

	// <-- FIELDS -->
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;			// Current active players
	std::map<LoggedUser, GameData> m_playersLeft;	// Players that left before the end of the game..
};