#pragma once

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
	const char* what() const noexcept override { return _message.c_str(); }

protected:
	std::string _message;
};