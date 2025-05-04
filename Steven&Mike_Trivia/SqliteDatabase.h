#pragma once

#include "IDatabase.h"
#include <vector>

// DEFINE CONSTS
#define DB_PATH "TriviaDB.db"

/*
								SqliteDatabase
	SqliteDatabase is a child class of IDatabase that represents a sqlite database.
*/

class SqliteDatabase : public IDatabase
{	
public:

	// Open the database
	virtual bool open() override;
	// Close the database
	virtual bool close() override;
	// Does the user exist in the Database?
	virtual int doesUserExist(const std::string& username) const override;
	// Does the password match for the userame in the database?
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) const override;
	// Add a new user to the database!
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

private:

	// <-- FIELDS -->
	sqlite3* _db;

	// <-- HELPER FUNCTIONS -->

	// The following functions will let us execute queries (both select queries and non select queries), and get the following results:
	// In case it is a non-select query, it will return whether the query executed successfully.
	// In case it is a select query - it will return whether the query resulted in at least one row - meaning, it will help us
	// with any functions that include checking if anything exists in the database.
	int executeQuery(const std::string& query, const std::vector<std::string>& params, bool isSelectQuery) const;
};