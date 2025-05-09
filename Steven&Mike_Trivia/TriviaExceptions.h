#pragma once

#include "UserValidity.h"
#include <exception>
#include <string>

/*
						TRIVIAEXCEPTIONS
	This header contains all of the different custom exceptions
	for our project Trivia.
	It includes all the different types, whether they are Byte exceptions,
	socket exceptions, or connection exceptions.
*/

/*
						PROJECTEXCEPTION
		This exception is a father class for all custom exceptions
		in our Trivia project.
*/

class ProjectException : public std::exception
{
public:
	ProjectException(const std::string& message) : _message(message) {}
	virtual const char* what() const noexcept override { return _message.c_str(); }

protected:
	std::string _message;
};

/*
							BYTEEXCEPTION
		ByteException is the father class for all exceptions that
		may come along when dealing with bytes using the Byte class.

		USED IN BYTE.CPP
*/

class ByteException : public ProjectException
{
public:
	ByteException(const std::string& message) : ProjectException(message) {}
};

// INVALID BYTE VALUE EXCEPTION - BYTE CAN ONLY HOLD VALUES 0 - 255
class InvalidByteValueException : public ByteException
{
public:
	InvalidByteValueException() : ByteException("Invalid byte value: a byte can only hold values 0-255!") {}
};

// INVALID BINARY STRING FORMAT EXCEPTION - BINARY STRING MUST ONLY CONTAIN THE CHARACTERS '0' AND '1'
class InvalidBinaryStringFormatException : public ByteException
{
public:
	InvalidBinaryStringFormatException() : ByteException("Invalid binary string format: binary string must only contain characters of '0' and '1'!") {}
};

// INVALID BINARY STRING LENGTH EXCEPTION - BINARY STRING MUST CONTAIN 8 OR LESS CHARACTERS (AS A BYTE CONTAINS AT MAX 8 BITS)
class InvalidBinaryStringLengthException : public ByteException
{
public:
	InvalidBinaryStringLengthException() : ByteException("Invalid binary string format: binary string must contain 8 or less characters!") {}
};

/*
								SOCKETEXCEPTION
		SocketException is the father class of all exceptions that may come
		along when using and dealing with sockets and communications.
*/

/*
								SIGNUPEXCEPTION
		SignupException is the father class of all exceptions that are involved in the
								signup process.

		USED IN LOGINMANAGER.CPP
*/

class SignupException : public ProjectException
{
public:
	SignupException(const std::string& message) : ProjectException(message) {}
};

// INVALID USERNAME EXCEPTION
class UsernameInvalidException : public SignupException
{
public:
	UsernameInvalidException(const std::string& username, usernameValidity validity);

protected:
	std::string _username;
};

// INVALID PASSWORD EXCEPTION
class PasswordInvalidException : public SignupException
{
public:
	PasswordInvalidException(const std::string& password, passwordValidity validity);

protected:
	std::string _password;
};

// INVALID EMAIL EXCEPTION
class EmailInvalidException : public SignupException
{
public:
	EmailInvalidException(const std::string& email, emailValidity validity);

protected:
	std::string _email;
};