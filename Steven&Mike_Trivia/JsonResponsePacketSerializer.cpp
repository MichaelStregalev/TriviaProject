#include "JsonResponsePacketSerializer.h"
#include "Byte.h"
#include <iostream>

/*
This is how responses in the protocol look like

CODE        DATA LENGHT         DATA
1 BYTE      4 BYTES             X BYTES
*/

// SERIALIZING ERROR RESPONSE
Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& response)
{
    nlohmann::json j;
    j[ERROR_MESSAGE_FIELD] = response.message;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(ERROR_RESPONSE_CODE, data);
}

// SERIALIZING SIGNUP RESPONSE
Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(SIGNUP_RESPONSE_CODE, data);
}

// SERIALIZING LOGIN RESPONSE
Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(LOGIN_RESPONSE_CODE, data);
}

Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(LOGOUT_RESPONSE_CODE, data);
}

Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;

    // Convert the vector of RoomData to a JSON array
    nlohmann::json roomsArray = nlohmann::json::array();

    for (const RoomData& room : response.rooms)
    {
        roomsArray.push_back(roomDataToJson(room));
    }

    j[ROOMS_FIELD] = roomsArray;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(GET_ROOMS_RESPONSE_CODE, data);
}

Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    nlohmann::json j;
    j[PLAYERS_FIELD] = response.players;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(GET_PLAYERS_IN_ROOMS_RESPONSE_CODE, data);
}

Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(JOIN_ROOM_RESPONSE_CODE, data);
}

Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(CREATE_ROOM_RESPONSE_CODE, data);
}

Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;
    j[SCORES_FIELD] = response.scores;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(GET_HIGHSCORE_RESPONSE_CODE, data);
}

Byte::Buffer JsonResponsePacketSerializer::serializeResponse(const GetStatisticsResponse& response)
{
    nlohmann::json j;
    j[STATUS_FIELD] = response.status;
    j[STATISTICS_FIELD] = response.statistics;

    std::string data = j.dump();    // Get the JSON as a string

    // Return the serialization buffer
    return baseSerialization(GET_STATISTICS_RESPONSE_CODE, data);
}

// BASE SERIALIZATION
Byte::Buffer JsonResponsePacketSerializer::baseSerialization(int code, const std::string& data)
{
    Byte::Buffer buffer;

    // First step: Add to the buffer the byte that represents the code of the response
    buffer.push_back(Byte(code));

    // Second step: Add to the buffer the 4 bytes that represent the data's length
    Byte::Buffer lengthOfData = Byte::intToBuffer(data.length());
    buffer.insert(buffer.end(), lengthOfData.begin(), lengthOfData.end());

    // Third step: Add to the buffer all of the bytes of the data
    Byte::Buffer dataBytes = Byte::stringToBuffer(data);
    buffer.insert(buffer.end(), dataBytes.begin(), dataBytes.end());

    // insert function lets us add the bytes in the buffers onto the main buffer.

    return buffer;
}

nlohmann::json JsonResponsePacketSerializer::roomDataToJson(const RoomData& room)
{
    return {
        {"id", room.id},
        {"name", room.name},
        {"maxPlayers", room.maxPlayers},
        {"numOfQuestionsInGame", room.numOfQuestionsInGame},
        {"timePerQuestion", room.timePerQuestion},
        {"roomStatus", room.roomStatus}
    };
}
