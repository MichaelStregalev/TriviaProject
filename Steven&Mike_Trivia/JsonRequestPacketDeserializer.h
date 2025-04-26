#pragma once

#include "Byte.h"
#include "json.hpp"
#include "Requests.h"

// DEFINES

#define USERNAME_FIELD	"userName"
#define PASSWORD_FIELD	"password"
#define EMAIL_FIELD		"email"

/*
					JsonResponsePacketDeerializer
	This namespace will allow us to deserialize any type of requests users
	have sent us, and parse them onto data requests that we can examine.
*/

namespace JsonRequestPacketDeserializer
{
	SignupRequest deserializeSignupRequest(const Byte::Buffer& buffer);
	LoginRequest deserializeLoginRequest(const Byte::Buffer& buffer);
}