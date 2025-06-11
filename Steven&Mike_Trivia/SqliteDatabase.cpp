#include "SqliteDatabase.h"
#include "TriviaExceptions.h"
//#include "Question.h"

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

int SqliteDatabase::doesUserExist(const std::string& username) const
{
	std::string query = "SELECT 1 FROM Users WHERE username = ?;";
	std::vector<std::string> params = { username };

	// Use executeQuery helper with isSelectQuery = true to check if the user exists
	return executeQuery(query, params, true);
}

int SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
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

std::vector<Question> SqliteDatabase::getQuestions(int num) const
{
	// The query that will let us get <num> questions
	std::string query = "SELECT * FROM Questions ORDER BY RANDOM() LIMIT " + std::to_string(num) + ";";
	std::vector<Question> questions;	// The list of questions

	int res = sqlite3_exec(_db, query.c_str(),
		[](void* data, int len, char** values, char** columns) -> int {
			auto returnValue = (std::vector<Question>*)data;
			Question q;		// Presenting the question

			// Going through the data
			for (int i = 0; i < len; i++)
			{
				if (std::string(columns[i]) == QUESTIONS_FIELD)
				{
					q.setQuestion(values[i]);
				}
				if (std::string(columns[i]) == ANSWERS_FIELD)
				{
					q.addAnswers(splitStringByComma(values[i]));
				}
				if (std::string(columns[i]) == CORRECT_FIELD)
				{
					q.addAnswer(values[i], true);
				}
			}

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

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username) const
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	// Query to find the average time of the user
	std::string query = "SELECT averageTime FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "');";
	float result = 0;

	int res = sqlite3_exec(_db, query.c_str(), callbackFloatValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username) const
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::string query = "SELECT correctAnswers FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result;

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username) const
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::string query = "SELECT totalQuestions FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result;

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username) const
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::string query = "SELECT totalGames FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result;

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);
	
	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

int SqliteDatabase::getPlayerScore(const std::string& username) const
{
	if (!doesUserExist(username))
	{
		throw UserDoesNotExistException(username);
	}

	std::string query = "SELECT score FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";
	int result;

	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return result;
}

std::map< std::string, int > SqliteDatabase::getHighScores() const
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

int SqliteDatabase::submitGameStatistics(GameData gm, const std::string& username) const
{
	int totalTime = gm.averageAnswerTime * (gm.correctAnswerCount + gm.wrongAnswerCount);
	int totalQuestions = gm.correctAnswerCount + gm.wrongAnswerCount;
	int scoreToAdd = (gm.correctAnswerCount * CORRECT_ANSWER) + (gm.wrongAnswerCount * WRONG_ANSWER);

	std::string queryStart = "UPDATE Statistics SET";
	std::string query1 = " totalTime = totalTime + " + std::to_string(totalTime) + ",";
	std::string query2 = " totalQuestions = totalQuestions + " + std::to_string(totalQuestions) + ",";
	std::string query3 = " correctAnswers = correctAnswers + " + std::to_string(gm.correctAnswerCount) + ",";
	std::string query4 = " totalGames = totalGames + 1,";
	std::string query5 = " score = score + " + std::to_string(scoreToAdd);
	std::string queryEnd = " WHERE userID = (SELECT ID FROM Users WHERE username = '" + username + "'); ";

	std::string query = queryStart + query1 + query2 + query3 + query4 + query5 + queryEnd;

	int res = sqlite3_exec(_db, query.c_str(), nullptr, nullptr, nullptr);

	if (res != SQLITE_OK)
	{
		throw FailedExecutionQueryException(query);
	}

	return 1;
}

// <-- PRIVATE HELPER METHODS -->

// THIS FUNCTION WILL ONLY LET US USE QUERIES THAT EITHER - CHECK IF SOMETHING EXISTS IN THE DATABASE, OR AN EXECUTION QUERY
int SqliteDatabase::executeQuery(const std::string& query, const std::vector<std::string>& params, bool isSelectQuery) const
{
	if (!_db)
	{
		throw DatabaseNotOpenException();
	}

	sqlite3_stmt* stmt;

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
		return (result == SQLITE_DONE) ? SUCCESSFUL_QUERY : UNSUCCESSFUL_QUERY; // Return 1 if the query was successful
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
	auto returnValue = (int*)data;

	if (len > 0)
	{
		(*returnValue) = std::stoi(values[FIRST_INDEX]);
	}

	return SQLITE_OK;
}

int SqliteDatabase::callbackFloatValue(void* data, int len, char** values, char** columns)
{
	auto returnValue = (float*)data;

	if (len > 0)
	{
		(*returnValue) = std::stof(values[FIRST_INDEX]);
	}

	return SQLITE_OK;
}