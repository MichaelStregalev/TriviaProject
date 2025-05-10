#include "StatisticManager.h"

// CONSTRUCTOR
StatisticManager::StatisticManager(IDatabase* db) : _db(db)
{
}

// GET THE HIGH SCORES
std::map<std::string, int> StatisticManager::getHighScore()
{
	return _db->getHighScores();
}

// GET THE USER STATISTICS
std::vector<double> StatisticManager::getUserStatistics(const std::string& username)
{
	std::vector<double> result;

	result.push_back(_db->getPlayerAverageAnswerTime(username));
	result.push_back(_db->getNumOfCorrectAnswers(username));
	result.push_back(_db->getNumOfTotalAnswers(username));
	result.push_back(_db->getNumOfPlayerGames(username));
	result.push_back(_db->getPlayerScore(username));

	return result;
}