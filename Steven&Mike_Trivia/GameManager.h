#pragma once

#include <vector>
#include "IDatabase.h"
#include "Game.h"

// <-- DEFINE CONSTS -->

#define MIN_QUESTION_COUNT		3
#define MAX_QUESTION_COUNT		20

/*
						GameManager
	GameManager allows us to manage ALL games in our server!
*/

class GameManager
{
public:

	// CONSTRUCTOR
	GameManager(IDatabase* db);

	// GETTERS
	Game* getGame(unsigned int gameId);

	// METHODS
	Game createGame(const Room& room, int questionCount);
	void deleteGame(unsigned int gameId);

private:

	// <-- FIELDS -->
	IDatabase* m_database;
	std::map<unsigned int, Game> m_games;

	// <-- PRIVATE METHODS -->
	void submitGameStatsToDatabase(const PlayerResult& playerResult, const LoggedUser& user);

};