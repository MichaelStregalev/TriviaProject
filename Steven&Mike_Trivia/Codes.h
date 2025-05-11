#pragma once

/*
				CODES
	This header includes all of the
	different codes for each of the
	responses and requests in our protocol.
*/

// <-- RESPONSE CODES -->

enum ResponseCodes { ERROR_RESPONSE_CODE = 0, SIGNUP_RESPONSE_CODE = 11, LOGIN_RESPONSE_CODE = 21 };

// <-- REQUEST CODES -->

enum RequestCodes { SIGNUP_REQUEST_CODE = 1, LOGIN_REQUEST_CODE = 2 };

// <-- HANDLERS & LOGIN MANAGER -->

#define SUCCESSFUL_LOGIN		99
#define SUCCESSFUL_SIGNUP		100

// <-- INDEXES IN BUFFER -->

#define REQUEST_ID_IN_BUFFER	0
#define DATA_LENGTH_START		1
#define DATA_LENGTH_END			4
#define DATA_START				5

// <-- ROOM CODES -->

enum RoomStatus { ROOM_OPEN = 200 };