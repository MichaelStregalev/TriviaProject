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
							SIGNUPEXCEPTION
		SignupException includes all kinds of exceptions that are involved in the
		signup process.
*/

class SignupException : public ProjectException
{
public:
	SignupException(const std::string& message) : ProjectException(message) {}

protected:
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