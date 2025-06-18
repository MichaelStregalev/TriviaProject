#include "GameRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "TriviaExceptions.h"

GameRequestHandler::GameRequestHandler(Game& game, LoggedUser user, GameManager& gameManager, RequestHandlerFactory& handler) :
	m_user(user), m_game(game), m_gameManager(gameManager), m_handlerFactory(handler)
{

}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	return request.requestId >= GET_GAME_RESULTS_REQUEST_CODE && request.requestId <= LEAVE_GAME_REQUEST_CODE;
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& request)
{
	// The result of the request..
	RequestResult result;

	switch (request.requestId)
	{
	case GET_GAME_RESULTS_REQUEST_CODE:
		result = getGameResults();
		break;

	case SUBMIT_ANSWER_REQUEST_CODE:
		result = submitAnswer(request);
		break;

	case GET_QUESTION_REQUEST_CODE:
		result = getQuestionForUser();
		break;

	case LEAVE_GAME_REQUEST_CODE:
		result = leaveGame();
		break;

	default:	// Shouldn't happen, as we check before if the request is relevant.
		break;
	}

	return result;
}

void GameRequestHandler::userLeftUnexpectedly()
{
	RequestResult leaveGameResult = leaveGame();
	m_handlerFactory.getLoginManager().logout(m_user.getUsername());
}

RequestResult GameRequestHandler::getQuestionForUser()
{
	RequestResult result;

	try
	{
		if (m_game.moreQuestionsForUser(m_user))
		{
			// Get the question for the user and the possible answers
			Question current = m_game.getCurrentQuestionForUser(m_user.getUsername());

			GetQuestionResponse response{ MORE_QUESTIONS, current.getQuestion(), current.getPossibleAnswers() };

			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = this;	// Keep in the game
		}
		else	// If the user has no more questions...
		{
			GetQuestionResponse response{ NO_MORE_QUESTIONS, "", std::map<unsigned int, std::string>()};

			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = this;	// Keep in the game
		}
		
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& request)
{
	RequestResult result;

	// Use the deserializer in order to get the info from the request
	SubmitAnswerRequest submitAnswerRequest = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(request.buffer);

	try
	{
		unsigned int status = 0;

		m_game.submitAnswer(m_user, submitAnswerRequest.answerId, submitAnswerRequest.answerTime);

		if (m_game.moreQuestionsForUser(m_user))
		{
			status = MORE_QUESTIONS;
		}
		else
		{
			status = NO_MORE_QUESTIONS;
		}

		SubmitAnswerResponse response{ status, m_game.getCurrentQuestionForUser(m_user).getCorrectId() };

		// After submitting the answer, we will make sure the user can get the next question - in case it did not finish the game.
		if (m_game.moreQuestionsForUser(m_user))
		{
			m_game.getNextQuestionForUser(m_user);
		}

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;	// Keep in the game
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}

RequestResult GameRequestHandler::getGameResults()
{
	RequestResult result;

	try
	{
		// If the game has not finished, an exception will be thrown.
		std::vector<PlayerResult> gameResults = m_game.getGameResults();

		GetGameResultsResponse response{ GET_GAME_RESULTS_RESPONSE_CODE, gameResults };

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;	// Keep in the game
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}

RequestResult GameRequestHandler::leaveGame()
{
	RequestResult result;

	try
	{
		LeaveGameResponse response{ LEAVE_GAME_RESPONSE_CODE };

		// Check if game still exists in GameManager
		if (m_gameManager.doesGameExist(m_game))
		{
			if (m_game.gameFinished())	// If the game exists, we will check if it has finished..
			{
				m_gameManager.deleteGame(m_game);  // If the game has finished, we will delete it!
			}
			else
			{
				m_game.removeUser(m_user);  // Safe to remove user from the game

				// Now - if after removing the user there are no current active users in the game - we should just delete the game!
				if (!m_game.thereAreActivePlayers())
				{
					m_gameManager.deleteGame(m_game);
				}
			}
		}
		// Else: game no longer exists — just continue without crashing

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);	// When leaving the game - return to the menu
	}
	catch (const std::exception& e)
	{
		ErrorResponse error{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(error);
		result.newHandler = this;
	}

	return result;
}
