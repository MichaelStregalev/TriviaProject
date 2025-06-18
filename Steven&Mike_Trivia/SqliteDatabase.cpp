#include "SqliteDatabase.h"
#include "TriviaExceptions.h"
#include "Question.h"
#include <sstream>     // for std::ostringstream
#include <iomanip>     // for std::fixed and std::setprecision
#include <cmath>

// <-- DEFINE CONSTS -->

// The names of the column fields in the database
#define QUESTIONS_FIELD		"question"
#define ANSWERS_FIELD		"answers"
#define CORRECT_FIELD		"correct"
#define USERNAME_FIELD		"username"
#define SCORE_FIELD			"score"

#define FIRST_INDEX			0			// the index of the first value

#define SUCCESSFUL_QUERY	1
#define UNSUCCESSFUL_QUERY	0

//Class functions
bool SqliteDatabase::open()
{
	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	// Try and open/create the database (will create in case it doesn't exist)
	int result = sqlite3_open(DB_PATH, &_db);
	if (result != SQLITE_OK)	// If the database couldn't open...
	{
		throw OpenDatabaseErrorException();
	}

	sqlite3_exec(_db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

	// Always attempt to create the database, will work in case the tables do not exist in the database.
	const char* createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            username TEXT NOT NULL,
            password TEXT NOT NULL,
            email TEXT NOT NULL,
			ID INTEGER PRIMARY KEY AUTOINCREMENT
        );
    )";

	result = sqlite3_exec(_db, createUsersTable, nullptr, nullptr, nullptr);	// Execute the creation of the users table

	if (result != SQLITE_OK)	// If an error occurred during the execution of the creation of the users table..
	{
		throw DatabaseSetupErrorException();
	}

	// Always attempt to create the database, will work in case the tables do not exist in the database.
	const char* createQuestionsTable = R"(
        CREATE TABLE IF NOT EXISTS Questions (
            question TEXT NOT NULL,
            answers TEXT NOT NULL,
            correct TEXT NOT NULL
        );
    )";

	result = sqlite3_exec(_db, createQuestionsTable, nullptr, nullptr, nullptr);	// Execute the creation of the questionsTable table

	if (result != SQLITE_OK)	// If an error occurred during the execution of the creation of the questions table..
	{
		throw DatabaseSetupErrorException();
	}

	// Always attempt to create the database, will work in case the tables do not exist in the database.
	const char* createStatisticsTable = R"(
        CREATE TABLE IF NOT EXISTS Statistics (
			userID INT NOT NULL,
			totalTime FLOAT NOT NULL DEFAULT 0,
			totalQuestions INT NOT NULL DEFAULT 0,
			averageTime FLOAT GENERATED ALWAYS AS (
			CASE 
            WHEN totalQuestions > 0 THEN totalTime/totalQuestions 
            ELSE 0 
			END) STORED,
			correctAnswers INT NOT NULL DEFAULT 0,
			totalGames INT NOT NULL DEFAULT 0,
			score INT NOT NULL DEFAULT 0,
			FOREIGN KEY (userID) REFERENCES Users(ID) ON DELETE CASCADE,
			PRIMARY KEY (userID)
		);
    )";

	result = sqlite3_exec(_db, createStatisticsTable, nullptr, nullptr, nullptr);	// Execute the creation of the StatisticsTable table

	if (result != SQLITE_OK)	// If an error occurred during the execution of the creation of the Statistics table..
	{
		throw DatabaseSetupErrorException();
	}

	std::cout << "Database opened and Users + Questions + Statistics tables verified.\n";
	return true;
}

bool SqliteDatabase::close()
{
	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	// If the database is opened...
	if (_db)
	{
		// Try and close the database..
		if (sqlite3_close(_db) == SQLITE_OK)
		{
			_db = nullptr;
			return true;
		}
		else
		{
			std::cerr << "Failed to close database: " << sqlite3_errmsg(_db) << std::endl;
			return false;
		}
	}

	return true; // Database was already closed
}

int SqliteDatabase::doesUserExist(const std::string& username)
{
	std::string query = "SELECT 1 FROM Users WHERE username = ?;";
	std::vector<std::string> params = { username };

	// Use executeQuery helper with isSelectQuery = true to check if the user exists
	return executeQuery(query, params, true);
}

int SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	std::string query = "SELECT 1 FROM Users WHERE username = ? AND password = ?;";
	std::vector<std::string> params = { username, password };

	// Use executeQuery helper with isSelectQuery = true to check if the password matches
	return executeQuery(query, params, true);
}

int SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	std::string query = "INSERT INTO Users (username, password, email) VALUES (?, ?, ?);";
	std::vector<std::string> params = { username, password, email };

	// Insert user
	int result = executeQuery(query, params, false);
	if (result != SUCCESSFUL_QUERY)
	{
		return UNSUCCESSFUL_QUERY; // failed to insert user
	}

	// Get the last inserted row ID (userID)
	int userID = static_cast<int>(sqlite3_last_insert_rowid(_db));

	// Insert into Statistics table with zero values
	std::string statQuery = "INSERT INTO Statistics (userID, totalTime, totalQuestions, correctAnswers, totalGames, score) VALUES (?, 0, 0, 0, 0, 0);";
	std::vector<std::string> statParams = { std::to_string(userID) };

	return executeQuery(statQuery, statParams, false); // Return result of statistics insertion
}

// <-- SATISTIC MANAGER FUNCTIONS -->

std::vector<Question> SqliteDatabase::getQuestions(int num)
{
	// The query that will let us get <num> questions
	std::string query = "SELECT * FROM Questions ORDER BY RANDOM() LIMIT " + std::to_string(num) + ";";
	std::vector<Question> questions;	// The list of questions

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	int res = sqlite3_exec(_db, query.c_str(),
		[](void* data, int len, char** values, char** columns) -> int {
			auto returnValue = (std::vector<Question>*)data;

			Question q;						// Presenting the question
			std::string correctAnswer;		// Presenting the correct answer
			std::vector<std::string> wrongAnswers;	// Presenting the wrong answers

			for (int i = 0; i < len; i++)
			{
				if (std::string(columns[i]) == QUESTIONS_FIELD)
				{
					q.setQuestion(values[i]);
				}
				else if (std::string(columns[i]) == ANSWERS_FIELD)
				{
					wrongAnswers = splitStringByComma(values[i]);
				}
				else if (std::string(columns[i]) == CORRECT_FIELD)
				{
					correctAnswer = values[i];
				}
			}

			q.setAnswers(wrongAnswers, correctAnswer);

			// Push the question onto the vector of questions
			returnValue->push_back(q);
			return SQLITE_OK;
		}, &questions, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	// Query to find the average time of the user
	std::string query = "SELECT averageTime FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "');";
	float result = 0;

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	int res = sqlite3_exec(_db, query.c_str(), callbackFloatValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::string query = "SELECT correctAnswers FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result;

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::string query = "SELECT totalQuestions FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result;

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	std::string query = "SELECT totalGames FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result;

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);
	
	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getPlayerScore(const std::string& username)
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	std::string query = "SELECT score FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result = 0;

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

std::map< std::string, int > SqliteDatabase::getHighScores()
{
	// Query of getting all of the high scores
	std::string query = R"(
		SELECT Users.username, Statistics.score
		FROM Statistics
		JOIN Users ON Statistics.UserId = Users.id
		ORDER BY Statistics.score DESC
		LIMIT 5;
		)";

	std::map<std::string, int> result;

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	int res = sqlite3_exec(_db, query.c_str(),
		[](void* data, int len, char** values, char** columns) -> int {
			auto returnValue = (std::map<std::string, int>*)data;
			std::string username;	// the username
			int score = 0;			// the user's score

			for (int i = 0; i < len; i++)
			{
				if (std::string(columns[i]) == USERNAME_FIELD)
				{
					username = values[i];
				}
				if (std::string(columns[i]) == SCORE_FIELD)
				{
					score = std::stoi(values[i]);
				}
			}

			returnValue->insert({ username, score });
			return SQLITE_OK;
		}, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::submitGameStatistics(const PlayerResult& result)
{
	if (!doesUserExist(result.username))
	{
		throw UserDoesNotExistException(result.username);
	}

	// Get the total questions answered by the user
	int totalQuestions = result.correctAnswerCount + result.wrongAnswerCount;

	// Get the total time that the user spent answering
	double totalTime = result.averageAnswerTime * totalQuestions;
	float roundedTime = roundf(totalTime * 100.0f) / 100.0f;

	std::ostringstream timeStream;
	timeStream << std::fixed << std::setprecision(2) << roundedTime;
	std::string roundedTimeStr = timeStream.str();

	std::cout << "[DEBUG] Starting transaction..." << std::endl;

	// Now, we will calculate the new score of the user..
	int newScore = calculateScore(result.username, result);

	std::cout << "[DEBUG] Calculated new score: " << newScore << std::endl;

	{
		std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

		int res = sqlite3_exec(_db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
		{
			std::cerr << "[ERROR] Failed to begin transaction: " << sqlite3_errmsg(_db) << std::endl;
			throw FailedExecutionQueryException("BEGIN TRANSACTION;");
		}
	}
	
	std::string updateStatsQuery = "UPDATE Statistics SET "
								   "totalTime = totalTime + ?, "
								   "totalQuestions = totalQuestions + ?, "
								   "correctAnswers = correctAnswers + ?, "
								   "totalGames = totalGames + 1 "
								   "WHERE userID = (SELECT ID FROM Users WHERE username = ?);";

	std::vector<std::string> params = 
	{
		roundedTimeStr,
		std::to_string(totalQuestions),
		std::to_string(result.correctAnswerCount),
		result.username
	};

	std::cout << "[DEBUG] Running stats update query:\n" << updateStatsQuery << std::endl;
	int updateResult = executeQuery(updateStatsQuery, params, false);

	if (updateResult != SUCCESSFUL_QUERY)
	{
		std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access
		std::cerr << "[ERROR] SQLite error: " << sqlite3_errmsg(_db) << std::endl;
		sqlite3_exec(_db, "ROLLBACK;", nullptr, nullptr, nullptr);
		throw FailedExecutionQueryException(updateStatsQuery);
	}

	// Update only the score
	std::string updateScoreQuery = "UPDATE Statistics SET score = ? WHERE userID = (SELECT ID FROM Users WHERE username = ?);";
	std::vector<std::string> scoreParams = { std::to_string(newScore), result.username };

	std::cout << "[DEBUG] Running score update query:\n" << updateScoreQuery << std::endl;
	int scoreResult = executeQuery(updateScoreQuery, scoreParams, false);	

	if (scoreResult != SUCCESSFUL_QUERY)
	{
		std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access
		sqlite3_exec(_db, "ROLLBACK;", nullptr, nullptr, nullptr);
		throw FailedExecutionQueryException(updateScoreQuery);
	}

	{
		std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

		int res = sqlite3_exec(_db, "COMMIT;", nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
		{
			std::cerr << "[ERROR] Failed to commit transaction: " << sqlite3_errmsg(_db) << std::endl;
			throw FailedExecutionQueryException("COMMIT;");
		}
	}

	return SUCCESSFUL_QUERY;
}

// <-- PRIVATE HELPER METHODS -->

// THIS FUNCTION WILL ONLY LET US USE QUERIES THAT EITHER - CHECK IF SOMETHING EXISTS IN THE DATABASE, OR AN EXECUTION QUERY
int SqliteDatabase::executeQuery(const std::string& query, const std::vector<std::string>& params, bool isSelectQuery)
{
	if (!_db)
	{
		throw DatabaseNotOpenException();
	}

	sqlite3_stmt* stmt;

	std::lock_guard<std::mutex> lock(_dbMutex); // Ensure serialized DB access

	// Prepare the query
	if (sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw FailedPreparationQueryException(query);
	}

	// Bind parameters to the prepared query
	for (int i = 0; i < params.size(); ++i)
	{
		sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT);
	}

	// Execute the query
	int result = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	// If the query is a SELECT query, check for rows
	if (isSelectQuery)
	{
		return (result == SQLITE_ROW) ? SUCCESSFUL_QUERY : UNSUCCESSFUL_QUERY; // Return 1 if a row was found, 0 if not
	}
	else	// If the query is a non select query - check if it was executed successfully..
	{
		return (result == SQLITE_DONE || result == SQLITE_OK) ? SUCCESSFUL_QUERY : UNSUCCESSFUL_QUERY; // Return 1 if the query was successful
	}
}

std::vector<std::string> SqliteDatabase::splitStringByComma(const std::string& string)
{
	std::vector<std::string> result;
	int start = 0;
	int end = string.find(',');

	while (end != std::string::npos)
	{
		result.push_back(string.substr(start, end - start));
		start = end + 1;
		end = string.find(',', start);
	}

	result.push_back(string.substr(start)); //For the last String

	return result;
}

// <-- CALLBACK FUNCTIONS -->

int SqliteDatabase::callbackIntValue(void* data, int len, char** values, char** columns)
{
	auto returnValue = static_cast<int*>(data);

	if (len > 0 && values[0] != nullptr)
	{
		(*returnValue) = std::stoi(values[FIRST_INDEX]);
	}

	return SQLITE_OK;
}

int SqliteDatabase::callbackFloatValue(void* data, int len, char** values, char** columns)
{
	auto returnValue = static_cast<float*>(data);

	if (len > 0 && values[0] != nullptr)
	{
		(*returnValue) = std::stof(values[FIRST_INDEX]);
	}

	return SQLITE_OK;
}

int SqliteDatabase::calculateScore(const std::string& username, const PlayerResult& result)
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	int correctAnswers = getNumOfCorrectAnswers(username) + result.correctAnswerCount;
	int totalAnswersOld = getNumOfTotalAnswers(username);
	int totalAnswersNew = totalAnswersOld + result.correctAnswerCount + result.wrongAnswerCount;
	int totalGames = getNumOfPlayerGames(username) + 1;
	float averageTime = ((getPlayerAverageAnswerTime(username) * totalAnswersOld) + 
						result.averageAnswerTime * (result.correctAnswerCount + result.wrongAnswerCount)) / totalAnswersNew;

	if (totalAnswersNew == 0 || totalGames == 0)
	{
		return 0; // Avoid division by zero or meaningless scores
	}

	// Get the amount of wrong answers
	int wrongAnswers = totalAnswersNew - correctAnswers;

	// Get the base points of the user - 
	// 10 GAINED FOR EACH CORRECT ANSWER, 2 LOST FOR EACH WRONG ANSWER
	int basePoints = (correctAnswers * SCORE_GAIN_CORRECT) - (wrongAnswers * SCORE_LOSE_WRONG);

	// Accuracy component: higher is better
	float accuracyRatio = 1.0f + (static_cast<float>(correctAnswers) / totalAnswersNew);

	// Speed component: faster is better, but clamp minimum time to avoid division by 0
	float speedScore = 1.0f + (1.0f / std::max(averageTime, 1.0f));

	// Games multiplier: experience gives small bonus
	float gameBonus = 1.0f + (std::log10(totalGames) * 0.25f);

	// FINAL SCORE
	int finalScore = static_cast<int>(std::round(basePoints * accuracyRatio * speedScore * gameBonus));

	return std::max(0, finalScore);		// Never return negative score
}
