#pragma once

#include "Byte.h"
#include "json.hpp"
#include "Requests.h"

// <-- DEFINE CONSTS FOR FIELDS -->

#define USERNAME_FIELD			"userName"
#define PASSWORD_FIELD			"password"
#define EMAIL_FIELD				"email"

#define ROOM_ID_FIELD			"roomId"
#define ROOM_NAME_FIELD			"roomName"
#define MAX_PLAYERS_FIELD		"maxPlayers"
#define QUESTION_COUNT_FIELD	"questionCount"
#define ANSWER_TIMEOUT_FIELD	"answerTimeout"

#define ANSWER_ID_FIELD			"answerId"


/*
					JsonRequestPacketDeserializer
	This namespace will allow us to deserialize any type of requests users
	have sent us, and parse them onto data requests that we can examine.
*/

namespace JsonRequestPacketDeserializer
{
	// <-- LoginManager deserializers
	SignupRequest deserializeSignupRequest(const Byte::Buffer& buffer);
	LoginRequest deserializeLoginRequest(const Byte::Buffer& buffer);

	// <-- Room deserializers
	GetPlayersInRoomRequest deserializeGetPlayersRequest(const Byte::Buffer& buffer);
	JoinRoomRequest deserializeJoinRoomRequest(const Byte::Buffer& buffer);
	CreateRoomRequest deserializeCreateRoomRequest(const Byte::Buffer& buffer);

	// <-- Game deserializers
	SubmitAnswerRequest deserializeSubmitAnswerRequest(const Byte::Buffer& buffer);
}