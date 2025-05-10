#pragma once

#include "SqliteDatabase.h"

class StatisticManager
{
public:

	StatisticManager(IDatabase* db);
	std::map<std::string, int> getHighScore();
	std::vector<float> getUserStatistics(const std::string& username);

private:

	IDatabase* _db;
};