#include "RoomMemberRequestHandler.h"

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return false;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& request)
{
    return RequestResult();
}
