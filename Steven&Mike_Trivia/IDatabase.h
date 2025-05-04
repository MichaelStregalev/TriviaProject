#pragma once

#include <iostream>
#include <string>
#include "sqlite3.h"

/*
								IDataBase
	IDataBase is an abstract class that represents a database class that will
	take care of all things needed in order to use a database.
*/

class IDatabase
{
public:

	// Open the database
	virtual bool open() = 0;												
	// Close the database
	virtual bool close() = 0;												
	// Check if a user exists
	virtual int doesUserExist(const std::string& username) const = 0;		
	// Check if a password matches to the user
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	// Add a new user to the database
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;

private:
};