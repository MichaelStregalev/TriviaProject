#include "SqliteDatabase.h"
#include <exception>

bool SqliteDatabase::open()
{
	// Try and open/create the database (will create in case it doesn't exist)
	int result = sqlite3_open(DB_PATH, &_db);
	if (result != SQLITE_OK)	// If the database couldn't open...
	{
		std::cerr << "Can't open database: " << sqlite3_errmsg(_db) << std::endl;
		return false;
	}

	// Always attempt to create the database, will work in case the tables do not exist in the database.
	const char* createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            username TEXT PRIMARY KEY,
            password TEXT NOT NULL,
            email TEXT NOT NULL
        );
    )";

	char* errMsg = nullptr;
	result = sqlite3_exec(_db, createUsersTable, nullptr, nullptr, &errMsg);	// Execute the creation of the users table

	if (result != SQLITE_OK)	// If an error occurred during the execution of the creation of the users table..
	{
		std::cerr << "SQL error during table creation: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}

	std::cout << "Database opened and Users table verified.\n";
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

int SqliteDatabase::executeQuery(const std::string& query, const std::vector<std::string>& params, bool isSelectQuery) const
{
	if (!_db)
	{
		throw std::runtime_error("Database connection is null.");
	}

	sqlite3_stmt* stmt;

	// Prepare the query
	if (sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		std::string errorMessage = "Failed to prepare the query: " + query;
		throw std::runtime_error(errorMessage);
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
