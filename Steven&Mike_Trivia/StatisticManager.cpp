#include "StatisticManager.h"

StatisticManager::StatisticManager(IDatabase* db)
{
	_db = db;
}

std::map<std::string, int> StatisticManager::getHighScore()
{
	return _db->getHighScores();
}

std::vector<float> StatisticManager::getUserStatistics(const std::string& username)
{
	std::vector<float> result;

	result.push_back(_db->getPlayerAverageAnswerTime(username));
	result.push_back(_db->getNumOfCorrectAnswers(username));
	result.push_back(_db->getNumOfTotalAnswers(username));
	result.push_back(_db->getNumOfPlayerGames(username));
	result.push_back(_db->getPlayerScore(username));

	return result;
}

