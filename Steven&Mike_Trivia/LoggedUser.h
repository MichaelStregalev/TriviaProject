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
		
	LoggedUser(const std::string& name);
	std::string getUsername() const;

private:
	std::string m_username;
};