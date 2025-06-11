#include "Game.h"
#include "TriviaExceptions.h"
#include "RoomManager.h"

Game::Game(const Room& room, std::vector<Question> questions) : m_gameId(room.getRoomData().id)
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
	m_players[user].averageAnswerTime = ((m_players[user].averageAnswerTime * (totalAnswers - 1)) + answerTime) / totalAnswers;
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
	m_playersLeft[user] = data;

	submitGameStatsToDatabase(data, user);
}
