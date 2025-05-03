#include "SqliteDatabase.h"

//----------------------- CallBacks -----------------------

int callbackCount(void* data, int len, char** values, char** columns)
{
	auto check = (int*)data;

	for (int i = 0; i < len; i++)
	{
		(*check) = std::stoi(values[i]);
	}

	return SQLITE_OK;
}

//----------------------- Class Functions -----------------------

bool SqliteDatabase::open()
{
	int res = sqlite3_open(DB_PATH, &_db);
	if (res != SQLITE_OK)
	{
		_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}
	return true;
}

bool SqliteDatabase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
	return true;
}

int SqliteDatabase::doesUserExist(const std::string& username) const
{
	int result = 0;
	std::string statment = "SELECT COUNT(*) FROM USERS WHERE UserName = '" + username + "';";

	const char* sqlStatment = statment.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatment, callbackCount, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}
	return result;
}

int SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	int result = 0;
	std::string statment = "SELECT COUNT(*) FROM USERS WHERE UserName = '" + username + "' AND PassWord = '" + password + "';";

	const char* sqlStatment = statment.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatment, callbackCount, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}
	return result;
}

int SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	/*
	TO DO:
	Add requiments: 
	Username (8 letters and above, atleast one upper case, blah blah blah)
	Password (8 letters blah blah blah)
	Email (Must have @)
	Each one returns another number
	for example:
	invalid userName (0)
	invalid Password (1)
	invalid Email (2)
	*/
	int result = 0;
	std::string statment = "INSERT INTO USERS('UserName', 'PassWord', 'Gmail') VALUES('" + username + "', '" + password + "', '" + email + "');";

	const char* sqlStatment = statment.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatment, callbackCount, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}
	return 1;
}
