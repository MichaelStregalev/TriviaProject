#include "Game.h"
#include "TriviaExceptions.h"
#include "RoomManager.h"

Game::Game()
{

}

Game::Game(const Room& room, std::vector<Question> questions)
{
	m_questions = questions;

	for (const auto& user : room.getAllUsers())
	{
		m_players[user] = GameData{m_questions[0], 0, 0, 0.0};
	}
}

Question Game::getCurrentQuestionForUser(const LoggedUser& user)
{
	if (m_players.find(user) == m_players.end())
	{
		throw UserNotInGameException(user.getUsername());
	}

	return m_players[user].currentQuestion;
}

std::vector<PlayerResult> Game::getGameResults()
{
	if (!gameFinished())
	{
		throw GameDidNotFinishException();
	}

	std::vector<PlayerResult> results;

	// Getting the results of all the ACTIVE players!
	for (const auto& pair : m_players)
	{
		PlayerResult result = PlayerResult{ pair.first.getUsername(), pair.second.correctAnswerCount, pair.second.wrongAnswerCount, pair.second.averageAnswerTime };

		results.push_back(result);
	}

	// Getting the results of all the players who left early...
	for (const auto& pair : m_playersLeft)
	{
		PlayerResult result = PlayerResult{ pair.first.getUsername(), pair.second.correctAnswerCount, pair.second.wrongAnswerCount, pair.second.averageAnswerTime };

		results.push_back(result);
	}

	return results;
}

void Game::getNextQuestionForUser(const LoggedUser& user)
{
	if (m_players.find(user) == m_players.end())
	{
		throw UserNotInGameException(user.getUsername());
	}

	// Getting the idnex of the current question that the user finished just now
	int indexOfQuestion = std::find(m_questions.begin(), m_questions.end(), m_players[user].currentQuestion) - m_questions.begin();

	if (indexOfQuestion + 1 >= m_questions.size())
	{
		throw NoMoreQuestionsException();
	}

	m_players[user].currentQuestion = m_questions[indexOfQuestion + 1];
}

void Game::submitAnswer(const LoggedUser& user, unsigned int answerId, double answerTime)
{
	if (m_players.find(user) == m_players.end())
	{
		throw UserNotInGameException(user.getUsername());
	}
	
	// Update GameData
	if (m_players[user].currentQuestion.getCorrectId() == answerId)
	{
		m_players[user].correctAnswerCount++;
	}
	else
	{
		m_players[user].wrongAnswerCount++;
	}

	// Update average answer time
	unsigned int totalAnswers = m_players[user].correctAnswerCount + m_players[user].wrongAnswerCount;

	if (totalAnswers > 0)
	{
		m_players[user].averageAnswerTime = ((m_players[user].averageAnswerTime * (totalAnswers - 1)) + answerTime) / totalAnswers;
	}
	else
	{
		m_players[user].averageAnswerTime = 0;
	}
}

void Game::removeUser(const LoggedUser& user)
{
	auto it = m_players.find(user);
	if (it == m_players.end())
	{
		throw UserNotInGameException(user.getUsername());
	}

	GameData data = it->second;
	m_players.erase(it);
	m_playersLeft[user] = data;		// Add the user to the players that left, in the end - the data will be added onto the statistics.
}

bool Game::gameFinished()
{
	// A game is finished when all active players (that did not leave the game in the middle) have answered all their questions.

	for (const auto& pair : m_players)
	{
		// Getting the total answers answered by the user
		unsigned int totalAnswers = pair.second.correctAnswerCount + pair.second.wrongAnswerCount;
		if (totalAnswers < m_questions.size())
		{
			return false;
		}
	}

	return true;
}

bool Game::moreQuestionsForUser(const LoggedUser& user)
{
	// Check if the user exists in the game data
	auto it = m_players.find(user.getUsername());
	if (it == m_players.end())
	{
		throw UserNotInGameException(user.getUsername());
	}

	return (it->second.wrongAnswerCount + it->second.correctAnswerCount) < m_questions.size();
}

bool Game::thereAreActivePlayers()
{
	return !m_players.empty();
}

bool Game::operator==(const Game& other) const
{
	return m_questions == other.m_questions &&
		m_players == other.m_players &&
		m_playersLeft == other.m_playersLeft;
}
