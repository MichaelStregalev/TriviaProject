#pragma once

#include "IRequestHandler.h"
#include "Game.h"
#include "LoggedUser.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

/*
						GameRequestHandler
	GameRequestHandler is a child class of IRequestHandler,
	it represents the handler of all that is related to the game system.
*/

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
public:

	// CONSTRUCTOR & DECONSTRUCTOR
	GameRequestHandler(Game& game, LoggedUser user, GameManager& gameManager, RequestHandlerFactory& handler);
	virtual ~GameRequestHandler() = default;

	// METHODS

	bool isRequestRelevant(const RequestInfo& request) const override;			// Is the request relevant?
	RequestResult handleRequest(const RequestInfo& request) override;			// Handle the request!

	void userLeftUnexpectedly() override;										// User left unexpectedly

private:

	// <-- FIELDS -->
	Game& m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;

	// <-- PRIVATE METHODS -->
	RequestResult getQuestionForUser();
	RequestResult submitAnswer(const RequestInfo& request);
	RequestResult getGameResults();
	RequestResult leaveGame();
};
