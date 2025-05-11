#include "JsonRequestPacketDeserializer.h"
#include "Requests.h"
#include "Byte.h"
#include "iostream"
#include "json.hpp"

// DESERIALIZING SIGNUP REQUEST JSON
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Byte::Buffer& buffer)
{
	std::string jsonString = Byte::deserializeBytesToString(buffer);

    try 
    {
        // Parse the JSON string onto a JSON object
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        // Create and populate SignupRequest
        SignupRequest request;
        request.userName = jsonData[USERNAME_FIELD].get<std::string>();
        request.password = jsonData[PASSWORD_FIELD].get<std::string>();
        request.email = jsonData[EMAIL_FIELD].get<std::string>();

        return request;
    }
    catch (const nlohmann::json::exception& e) 
    {
        throw std::invalid_argument("Invalid SignupRequest JSON format!");
    }
}

// DESERIALIZING LOGIN REQUEST JSON
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Byte::Buffer& buffer)
{
	std::string jsonString = Byte::deserializeBytesToString(buffer);

    try 
    {
        // Parse the JSON string onto a JSON object
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        // Create and populate LoginRequest
        LoginRequest request;
        request.userName = jsonData[USERNAME_FIELD].get<std::string>();
        request.password = jsonData[PASSWORD_FIELD].get<std::string>();

        return request;
    }
    catch (const nlohmann::json::exception& e)
    {
        throw std::invalid_argument("Invalid LoginRequest JSON format!");
    }
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const Byte::Buffer& buffer)
{
    std::string jsonString = Byte::deserializeBytesToString(buffer);

    try
    {
        // Parse the JSON string onto a JSON object
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        // Create and populate GetPlayersInRoomRequest
        GetPlayersInRoomRequest request;
        request.roomId = jsonData[ROOM_ID_FIELD].get<unsigned int>();

        return request;
    }
    catch (const nlohmann::json::exception& e)
    {
        throw std::invalid_argument("Invalid LoginRequest JSON format!");
    }
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Byte::Buffer& buffer)
{
    std::string jsonString = Byte::deserializeBytesToString(buffer);

    try
    {
        // Parse the JSON string onto a JSON object
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        // Create and populate JoinRoomRequest
        JoinRoomRequest request;
        request.roomId = jsonData[ROOM_ID_FIELD].get<unsigned int>();

        return request;
    }
    catch (const nlohmann::json::exception& e)
    {
        throw std::invalid_argument("Invalid LoginRequest JSON format!");
    }
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Byte::Buffer& buffer)
{
    std::string jsonString = Byte::deserializeBytesToString(buffer);

    try
    {
        // Parse the JSON string onto a JSON object
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        // Create and populate CreateRoomRequest
        CreateRoomRequest request;
        request.roomName = jsonData[ROOM_NAME_FIELD].get<std::string>();
        request.maxPlayers = jsonData[MAX_PLAYERS_FIELD].get<unsigned int>();
        request.questionCount = jsonData[QUESTION_COUNT_FIELD].get<unsigned int>();
        request.answerTimeout = jsonData[ANSWER_TIMEOUT_FIELD].get<unsigned int>();

        return request;
    }
    catch (const nlohmann::json::exception& e)
    {
        throw std::invalid_argument("Invalid LoginRequest JSON format!");
    }
}
