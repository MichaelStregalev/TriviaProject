#pragma once

#include "IDatabase.h"
#include "Question.h"

// DEFINE CONSTS
#define DB_PATH "TriviaDB.db"

/*
								SqliteDatabase
	SqliteDatabase is a child class of IDatabase that represents a sqlite database.
*/

class SqliteDatabase : public IDatabase
{	
public:

	// DECONSTRUCTOR
	virtual ~SqliteDatabase() = default;

	// Open the database
	bool open() override;
	// Close the database
	bool close() override;
	// Does the user exist in the Database?
	int doesUserExist(const std::string& username) const override;
	// Does the password match for the userame in the database?
	int doesPasswordMatch(const std::string& username, const std::string& password) const override;
	// Add a new user to the database!
	int addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

	// <-- STATISTIC MANAGER FUNCTIONS -->

	// Get Questions
	std::vector<Question> getQuestions(int num) const override;
	// Get player's average answer time
	float getPlayerAverageAnswerTime(const std::string& username) const override;
	// Get the player's number of correct answers
	int getNumOfCorrectAnswers(const std::string& username) const override;
	// Get the total amount of questions the player answered
	int getNumOfTotalAnswers(const std::string& username) const override;
	// Get the total amount of games the player played
	int getNumOfPlayerGames(const std::string& username) const override;
	// Get the player's score
	int getPlayerScore(const std::string& username) const override;
	// Get the high scores
	std::map<std::string, int> getHighScores() const override;
	// Update the DataBase according to Game Data
	int submitGameStatistics(GameData gm, const std::string& username) const override;

private:

	// <-- FIELDS -->
	sqlite3* _db;

	// <-- HELPER FUNCTIONS -->

	// The following functions will let us execute queries (both select queries and non select queries), and get the following results:
	// In case it is a non-select query, it will return whether the query executed successfully.
	// In case it is a select query - it will return whether the query resulted in at least one row - meaning, it will help us
	// with any functions that include checking if anything exists in the database.
	int executeQuery(const std::string& query, const std::vector<std::string>& params, bool isSelectQuery) const;

	// This function is responsible for getting a string, and splitting it onto a vector of strings - where each
	// element in the vector is the parts of the original string - as they are separated by a comma.
	static std::vector<std::string> splitStringByComma(const std::string& string);

	// Callback function for integer results in the database
	static int callbackIntValue(void* data, int len, char** values, char** columns);

	// Callback function for float results in the database
	static int callbackFloatValue(void* data, int len, char** values, char** columns);
};