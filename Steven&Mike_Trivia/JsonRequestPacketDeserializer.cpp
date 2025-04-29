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

        // Create and populate SignupRequest
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