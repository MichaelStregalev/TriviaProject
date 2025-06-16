#pragma once

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include "Question.h"
#include "sqlite3.h"
#include "GameData.h"
#include "PlayerResult.h"

/*
								IDataBase
	IDataBase is an abstract class that represents a database class that will
	take care of all things needed in order to use a database.
*/

class IDatabase
{
public:

	// Open the database
	virtual bool open() = 0;												
	// Close the database
	virtual bool close() = 0;												
	// Check if a user exists
	virtual int doesUserExist(const std::string& username) const = 0;		
	// Check if a password matches to the user
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	// Add a new user to the database
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;

	// <-- STATISTIC MANAGER FUNCTIONS -->

	// Get Questions
	virtual std::vector<Question> getQuestions(int num) const = 0;
	// Get player's average answer time
	virtual float getPlayerAverageAnswerTime(const std::string& username) const = 0;
	// Get the player's number of correct answers
	virtual int getNumOfCorrectAnswers(const std::string& username) const = 0;
	// Get the total amount of questions the player answered
	virtual int getNumOfTotalAnswers(const std::string& username) const = 0;
	// Get the total amount of games the player played
	virtual int getNumOfPlayerGames(const std::string& username) const = 0;
	// Get the player's score
	virtual int getPlayerScore(const std::string& username) const = 0;
	// Get the high scores
	virtual std::map<std::string, int> getHighScores() const = 0;
	// Update the DataBase according to Game Data
	virtual int submitGameStatistics(const PlayerResult& result) const = 0;


private:

};