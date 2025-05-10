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
	InvalidByteValueException() : ByteException("Invalid byte value: Byte can only hold values 0-255!") {}
};

/*
						BINARYSTRINGEXCEPTION
		BinaryStringException includes all kinds of exceptions that may
		come along when dealing with binary strings in the Byte class.
*/

class BinaryStringException : public ByteException
{
public:
	BinaryStringException(const std::string& error) : ByteException("Invalid Binary String Format: " + error) {}
};

// INVALID BINARY STRING FORMAT EXCEPTION - BINARY STRING MUST ONLY CONTAIN THE CHARACTERS '0' AND '1'
class InvalidBinaryStringFormatException : public BinaryStringException
{
public:
	InvalidBinaryStringFormatException() : BinaryStringException("Must only contain characters of '0' and '1'!") {}
};

// INVALID BINARY STRING LENGTH EXCEPTION - BINARY STRING MUST CONTAIN 8 OR LESS CHARACTERS (AS A BYTE CONTAINS AT MAX 8 BITS)
class InvalidBinaryStringLengthException : public BinaryStringException
{
public:
	InvalidBinaryStringLengthException() : BinaryStringException("Must contain 8 or less characters!") {}
};

/*
								SOCKETEXCEPTION
		SocketException is the father class of all exceptions that may come
		along when using and dealing with sockets and communications.

		USED IN COMMUNICATOR.CPP
*/

class SocketException : public ProjectException
{
public:
	SocketException(const std::string& message) : ProjectException(message) {}
};

/*
							SOCKETSETUPEXCEPTION
		SocketSetupException includes all kinds of exceptions that may come
					when setting up the sockets in the server.
*/

class SocketSetupException : public SocketException
{
public:
	SocketSetupException(const std::string& error) : SocketException("An error occurred while trying to " + error) {}
};

// SOCKET CREATION ERROR EXCEPTION
class SocketCreationErrorException : public SocketSetupException
{
public:
	SocketCreationErrorException() : SocketSetupException("create a socket.") {}
};

// SOCKET BINDING ERROR EXCEPTION
class SocketBindingErrorException : public SocketSetupException
{
public:
	SocketBindingErrorException() : SocketSetupException("bind the socket.") {}
};

// SOCKET LISTENING ERROR EXCEPTION
class SocketListeningErrorException : public SocketSetupException
{
public:
	SocketListeningErrorException() : SocketSetupException("listen for incoming connections.") {}
};

/*
								WSAEXCEPTION
		WSAException includes all kinds of exceptions that may come
			when dealing with WSA - like the setup of WSA.
*/

class WSAException : public SocketException
{
public:
	WSAException(const std::string& error) : SocketException("WSA Error: " + error) {}
};

// WSA SETUP ERROR EXCEPTION
class WSASetUpErrorException : public WSAException
{
public:
	WSASetUpErrorException() : WSAException("WSA Setup failed.") {}
};

/*
							SOCKETCOMMUNICATIONEXCEPTION
		SocketCommunicationError includes all kinds of exceptions that may come
			when trying to communicate with the clients using the sockets.
*/

class SocketCommunicationException : public SocketException
{
public:
	SocketCommunicationException(const std::string& error) : SocketException("Socket Communication Error: " + error) {}
};

// USER CONNECTION ERROR EXCEPTION
class UserConnectionErrorException : public SocketCommunicationException
{
public:
	UserConnectionErrorException() : SocketCommunicationException("Client failed to connect to the server.") {}
};

// SENDING MESSAGE VIA SOCKET ERROR
class SendingMessageErrorException : public SocketCommunicationException
{
public:
	SendingMessageErrorException() : SocketCommunicationException("Failed to send a message through the socket.") {}
};

// RECEIVING MESSAGE VIA SOCKET ERROR
class ReceivingMessageErrorException : public SocketCommunicationException
{
public:
	ReceivingMessageErrorException() : SocketCommunicationException("Failed to receive a message through the socket.") {}
};

/*
								REQUESTEXCEPTION
		RequestException includes all kinds of exceptions that may come
					when dealing with requests from the user.
*/

class RequestException : public SocketException
{
public:
	RequestException(const std::string& error) : SocketException("Request Exception: " + error) {}
};

// REQUEST NOT RELEVANT
class RequestNotRelevantException : public RequestException
{
public:
	RequestNotRelevantException() : RequestException("The received request is not relevant or supported.") {}
};

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