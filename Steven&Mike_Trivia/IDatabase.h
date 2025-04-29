#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include "sqlite3.h"

class IDatabase
{
public:

	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual int doesUserExist(std::string) = 0;
	virtual int doesPasswordMatch(std::string, std::string) = 0;
	virtual int addNewUser(std::string, std::string, std::string) = 0;

private:

};