#pragma once

#include <set>
#include "IDatabase.h"
#include "LoggedUser.h"

class LoginManager
{

public:
	
	LoginManager(IDatabase* db);

	bool signup(std::string userName, std::string passWord, std::string email);
	bool login(std::string userName, std::string passWord);
	bool logout(std::string userName);

private:

	IDatabase* m_database;
	std::set<LoggedUser> m_loggedUsers;
};
