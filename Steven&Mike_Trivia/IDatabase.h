#pragma once

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include "Structs.h"
#include "sqlite3.h"

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
	// Get Questions
	virtual std::list<Question> getQuestions(int num) = 0;
	virtual float getPlayerAverageAnswerTime(std::string userName) = 0;
	virtual int getNumOfCorrectAnswers(std::string userName) = 0;
	virtual int getNumOfTotalAnswers(std::string userName) = 0;
	virtual int getNumOfPlayerGames(std::string userName) = 0;
	virtual int getPlayerScore(std::string userName) = 0;
	virtual std::map<std::string, int> getHighScores() = 0;

private:
};