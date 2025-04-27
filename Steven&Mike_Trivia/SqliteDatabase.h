#pragma once

#include "IDatabase.h"

const std::string DB_Path = "TriviaDB.db";
static sqlite3* db;

class SqliteDatabase : public IDatabase
{	
public:

	virtual bool open() override;
	virtual bool close() override;
	virtual int doesUserExist(std::string userName) override;
	virtual int doesPasswordMatch(std::string userName, std::string passWord) override;
	virtual int addNewUser(std::string userName, std::string passWord, std::string email) override;

private:


};
