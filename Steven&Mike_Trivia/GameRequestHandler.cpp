#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(RequestHandlerFactory& handler) : m_handlerFactory(handler)
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	return false;
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& request)
{
	return RequestResult();
}
