#include "SqliteDatabase.h"
#include "TriviaExceptions.h"

//Helper functions
std::vector<std::string> splitStringByComma(const std::string& string) 
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

//CallBacks
int callbackQuestions(void* data, int len, char** values, char** columns)
{
	auto returnValue = (std::list<Question>*)data;
	std::vector<std::string> wrongAnswers;
	Question q;

	for (int i = 0; i < len; i++)
	{
		if (std::string(columns[i]) == "question")
		{
			q.question = (values[i]);
		}
		if (std::string(columns[i]) == "answers")
		{
			q.wrong = splitStringByComma(values[i]);
		}
		if (std::string(columns[i]) == "correct")
		{
			q.correct = (values[i]);
		}
	}

	(*returnValue).push_back(q);

	return SQLITE_OK;
}

int callbackFloatValue(void* data, int len, char** values, char** columns)
{
	auto returnValue = (float*)data;

	for (int i = 0; i < len; i++)
	{
		(*returnValue) = std::stof(values[i]);
	}

	return SQLITE_OK;
}

int callbackIntValue(void* data, int len, char** values, char** columns)
{
	auto returnValue = (int*)data;

	for (int i = 0; i < len; i++)
	{
		(*returnValue) = std::stoi(values[i]);
	}

	return SQLITE_OK;
}

int callbackMapHighScore(void* data, int len, char** values, char** columns)
{
	auto returnValue = (std::map<std::string, int>*)data;
	std::string username;
	int score;

	for (int i = 0; i < len; i++)
	{
		if (std::string(columns[i]) == "username")
		{
			username = values[i];
		}
		if (std::string(columns[i]) == "score")
		{
			score = std::stoi(values[i]);
		}
	}

	(*returnValue).insert({ username, score });
	
	return SQLITE_OK;
}

//Class functions
bool SqliteDatabase::open()
{
	// Try and open/create the database (will create in case it doesn't exist)
	int result = sqlite3_open(DB_PATH, &_db);
	if (result != SQLITE_OK)	// If the database couldn't open...
	{
		throw OpenDatabaseErrorException();
	}

	// Always attempt to create the database, will work in case the tables do not exist in the database.
	const char* createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            username TEXT NOT NULL,
            password TEXT NOT NULL,
            email TEXT NOT NULL,
			ID INTEGER PRIMARY KEY AUTOINCREMENT
        );
    )";

	char* errMsg = nullptr;
	result = sqlite3_exec(_db, createUsersTable, nullptr, nullptr, &errMsg);	// Execute the creation of the users table

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

	errMsg = nullptr;
	result = sqlite3_exec(_db, createQuestionsTable, nullptr, nullptr, &errMsg);	// Execute the creation of the questionsTable table

	if (result != SQLITE_OK)	// If an error occurred during the execution of the creation of the questions table..
	{
		std::cerr << "SQL error during table creation: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
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
            ELSE NULL 
        END
    ) STORED,
    correctAnswers INT NOT NULL DEFAULT 0,
    totalGames INT NOT NULL DEFAULT 0,
    score INT NOT NULL DEFAULT 0,
    FOREIGN KEY (userID) REFERENCES Users(ID),
    PRIMARY KEY (userID)
);
    )";

	errMsg = nullptr;
	result = sqlite3_exec(_db, createStatisticsTable, nullptr, nullptr, &errMsg);	// Execute the creation of the StatisticsTable table

	if (result != SQLITE_OK)	// If an error occurred during the execution of the creation of the Statistics table..
	{
		std::cerr << "SQL error during table creation: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
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
	const std::string query = "SELECT 1 FROM Users WHERE username = ?;";
	std::vector<std::string> params = { username };

	// Use executeQuery helper with isSelectQuery = true to check if the user exists
	return executeQuery(query, params, true);
}

int SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	const std::string query = "SELECT 1 FROM Users WHERE username = ? AND password = ?;";
	std::vector<std::string> params = { username, password };

	// Use executeQuery helper with isSelectQuery = true to check if the password matches
	return executeQuery(query, params, true);
}

int SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	const std::string query = "INSERT INTO Users (username, password, email) VALUES (?, ?, ?);";
	std::vector<std::string> params = { username, password, email };

	// Use executeQuery helper with isSelectQuery = false to add the new user
	return executeQuery(query, params, false);
}

std::list<Question> SqliteDatabase::getQuestions(int num)
{
	const std::string query = "SELECT * FROM questions ORDER BY RANDOM() LIMIT " + std::to_string(num) + ";";
	std::list<Question> questions;

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, query.c_str(), callbackQuestions, &questions, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}

	return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(std::string userName)
{
	const std::string query = "SELECT averageTime FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + userName + "'); ";
	float result;

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, query.c_str(), callbackFloatValue, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}

	return result;
}

int SqliteDatabase::getNumOfCorrectAnswers(std::string userName)
{
	const std::string query = "SELECT correctAnswers FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + userName + "'); ";
	int result;

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}

	return result;
}

int SqliteDatabase::getNumOfTotalAnswers(std::string userName)
{
	const std::string query = "SELECT totalQuestions FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + userName + "'); ";
	int result;

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}

	return result;
}

int SqliteDatabase::getNumOfPlayerGames(std::string userName)
{
	const std::string query = "SELECT totalGames FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + userName + "'); ";
	int result;

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}

	return result;
}

int SqliteDatabase::getPlayerScore(std::string userName)
{
	const std::string query = "SELECT score FROM Statistics WHERE userID = (SELECT ID FROM Users WHERE username = '" + userName + "'); ";
	int result;

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, query.c_str(), callbackIntValue, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}

	return result;
}

std::map< std::string, int > SqliteDatabase::getHighScores()
{
	const std::string query = R"(
		SELECT Users.username, Statistics.score
		FROM Statistics
		JOIN Users ON Statistics.UserId = Users.id
		ORDER BY Statistics.score DESC
		LIMIT 5;
		)";
	std::map<std::string, int> result;

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, query.c_str(), callbackMapHighScore, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}

	return result;
}

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
		sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
	}

	// Execute the query
	int result = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	// If the query is a SELECT query, check for rows
	if (isSelectQuery)
	{
		return (result == SQLITE_ROW) ? 1 : 0; // Return 1 if a row was found, 0 if not
	}
	else	// If the query is a non select query - check if it was executed successfully..
	{
		return (result == SQLITE_DONE) ? 1 : 0; // Return 1 if the query was successful
	}
}
