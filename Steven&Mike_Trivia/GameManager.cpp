#include "GameManager.h"
#include "TriviaExceptions.h"

GameManager::GameManager(IDatabase* db, RoomManager& manager) : m_database(db), m_roomManager(manager)
{
}

Game* GameManager::getGame(unsigned int gameId)
{
    auto it = m_games.find(gameId);

    // If game exists, return a pointer to it; otherwise, return nullptr
    if (it != m_games.end())
    {
        return const_cast<Game*>(&it->second);  // Non-const pointer to Game
    }

    // If the game has not been found, throw an exception

    throw RoomDoesNotExistException(gameId);
}

Game GameManager::createGame(const Room& room, int questionCount)
{
    if (questionCount < MIN_QUESTION_COUNT || questionCount > MAX_QUESTION_COUNT)
    {
        throw InvalidQuestionCountException();
    }

    std::vector<Question> questions = m_database->getQuestions(questionCount);

    m_games[room.getRoomData().id] = Game(room, questions);

    return m_games[room.getRoomData().id];
}

void GameManager::deleteGame(const Game& game)
{
    // Try and find the game, and delete it!
    for (const auto& pair : m_games)
    {
        if (pair.second == game)
        {
            deleteGame(pair.first);
            return;
        }
    }

    throw GameDoesNotExistException();
}

void GameManager::deleteGame(unsigned int gameId)
{
    auto it = m_games.find(gameId);

    // Make sure that the game does exist...
    if (it != m_games.end())
    {
        Game& game = it->second;

        if (!game.gameFinished())
        {
            throw GameDidNotFinishException();
        }

        // Submit stats for all the players that were in the game
        std::vector<PlayerResult> results = game.getGameResults();

        for (int i = 0; i < results.size(); ++i)
        {
            submitGameStatsToDatabase(results[i]);
        }

        m_games.erase(it);
        m_roomManager.deleteRoom(gameId);
    }
    else
    {
        throw GameDoesNotExistException(gameId);
    }
}

bool GameManager::doesGameExist(const Game& game)
{
    for (const auto& pair : m_games)
    {
        if (pair.second == game)
        {
            return true;
        }
    }
    return false;
}

void GameManager::submitGameStatsToDatabase(const PlayerResult& playerResult)
{
    m_database->submitGameStatistics(playerResult);
}
