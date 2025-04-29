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
	int res = sqlite3_open(DB_Path.c_str(), &db);
	if (res != SQLITE_OK)
	{
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}
	return true;
}

bool SqliteDatabase::close()
{
	sqlite3_close(db);
	db = nullptr;
	return true;
}

int SqliteDatabase::doesUserExist(std::string userName)
{
	int result = 0;
	std::string statment = "SELECT COUNT(*) FROM USERS WHERE UserName = '" + userName + "';";

	const char* sqlStatment = statment.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatment, callbackCount, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}
	return result;
}

int SqliteDatabase::doesPasswordMatch(std::string userName, std::string passWord)
{
	int result = 0;
	std::string statment = "SELECT COUNT(*) FROM USERS WHERE UserName = '" + userName + "' AND PassWord = '" + passWord + "';";

	const char* sqlStatment = statment.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatment, callbackCount, &result, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}
	return result;
}

int SqliteDatabase::addNewUser(std::string userName, std::string passWord, std::string email)
{
	//Username (8 letters and above, atleast one upper case)
	if (!(userName.length() >= 8) || !(hasUpperCase(userName)))
	{
		return 1;
	}

	//Password (8 letters)
	if (!(passWord.length() >= 8))
	{
		return 2;
	}

	//Email (Must have @gmail.com)
	if (!(email.find("@gmail.com") != std::string::npos))
	{
		return 3;
	}

	//No problem was found, add new user
	std::string statment = "INSERT INTO USERS('UserName', 'PassWord', 'Gmail') VALUES('" + userName + "', '" + passWord + "', '" + email + "');";

	const char* sqlStatment = statment.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatment, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "Command is INVALID" << std::endl;
	}
	return 0; //SignUp Successfull
}

//Custom functions
bool SqliteDatabase::hasUpperCase(const std::string& str)
{
	for (char c : str)
	{
		if (isupper(c))
		{
			return true;
		}
	}
	return false;
}
