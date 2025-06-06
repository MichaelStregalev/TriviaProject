#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include "UserValidity.h"
#include <set>

/*
							LoginManager
	LoginManager is a class that will be responsible for the users'
	signup, login, and logout features.
*/

class LoginManager
{

public:
	
	// CONSTRUCTOR
	LoginManager(IDatabase* db);

	// DECONSTRUCTOR
	~LoginManager();

	// SIGNUP
	bool signup(const std::string& username, const std::string& password, const std::string& email);
	// LOGIN
	bool login(const std::string& username, const std::string& password);
	// LOGOUT
	bool logout(const std::string& username);

private:

	// <-- FIELDS -->
	IDatabase* m_database;
	std::set<LoggedUser> m_loggedUsers;

	// <-- PRIVATE HELPER FUNCTIONS -->
	void validateUsername(const std::string& username);
	void validatePassword(const std::string& password, const std::string& username);
	void validateEmail(const std::string& email);
};
