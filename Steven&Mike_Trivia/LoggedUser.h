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

	// COPY CONSTRUCTOR
	LoggedUser(const LoggedUser& other);

	// GETTER OF USERNAME
	std::string getUsername() const;

	// OPERATORS

	// operator < is needed in order to use the algorithm find()
	bool operator<(const LoggedUser& other) const;

private:

	// <-- FIELDS -->
	std::string m_username;
};