#include "MenuRequestHandler.h"

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& request)
{
	return RequestResult();
}
