#pragma once

/*
				CODES
	This header includes all of the
	different codes for each of the
	responses and requests in our protocol.
*/

// <-- RESPONSE CODES -->

#define ERROR_RESPONSE_CODE		0
#define SIGNUP_RESPONSE_CODE	11
#define LOGIN_RESPONSE_CODE		21

// <-- REQUEST CODES -->

#define SIGNUP_REQUEST_CODE		1
#define LOGIN_REQUEST_CODE		2

// <-- HANDLERS -->

#define SUCCESSFUL_LOGIN		99
#define SUCCESSFUL_SIGNUP		100