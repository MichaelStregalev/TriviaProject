#include "RoomAdminRequestHandler.h"

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& request)
{
    return RequestResult();
}
