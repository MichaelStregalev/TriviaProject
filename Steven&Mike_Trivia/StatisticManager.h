#pragma once

#include "SqliteDatabase.h"

/*
								StatisticManager
	StatisticManager is a class that will manage all of the satistics of users.
*/

class StatisticManager
{
public:

	// CONSTRUCTOR
	StatisticManager(IDatabase* db);

	// <-- METHODS -->
	std::map<std::string, int> getHighScore();
	std::vector<double> getUserStatistics(const std::string& username);

private:

	// <-- FIELDS -->
	IDatabase* _db;
};