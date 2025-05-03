#pragma once

#include "IDatabase.h"

// DEFINE CONSTS
#define DB_PATH "TriviaDB.db"

/*
								SqliteDatabase
	SqliteDatabase is a child class of IDatabase that represents a sqlite database.
*/

class SqliteDatabase : public IDatabase
{	
public:

	virtual bool open() override;
	virtual bool close() override;
	virtual int doesUserExist(const std::string& username) const override;
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

private:

	sqlite3* _db;

};
