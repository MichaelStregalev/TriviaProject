#pragma once
#include <iostream>
#include <string>

/*
					LoggedUser
	This class represents a logged user in the server.
*/

class LoggedUser
{
public:

	// CONSTRUCTOR
	LoggedUser(const std::string& name);

	// GETTER OF USERNAME
	std::string getUsername() const;

	// OPERATOR	

	// operator < is needed in order to use the algorithm find()
	bool operator<(const LoggedUser& other) const;

private:

	// <-- FIELDS -->
	std::string m_username;
};