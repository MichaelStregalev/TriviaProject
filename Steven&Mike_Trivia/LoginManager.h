#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include "UserValidity.h"
#include <set>

// DEFINES CONSTS
#define		LOGIN_SUCCESSFUL		1
#define		PASSWORD_DONT_MATCH		0
#define		ALREADY_CONNECTED		-1

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

	// SIGNUP
	bool signup(const std::string& username, const std::string& password, const std::string& email);
	// LOGIN (1 = login successful, 0 = passwords do NOT match, -1 = user already connected)
	int login(const std::string& username, const std::string& password);
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
