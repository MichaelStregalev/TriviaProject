#pragma once

#include "UserValidity.h"
#include <exception>
#include <string>

// <-- DOWN BELOW YOU CAN SEE THE HIERARCHY OF ALL THE EXCEPTIONS -->

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

// USER DOES NOT EXIST EXCEPTION
class UserDoesNotExistException : public SignupException
{
public:
	UserDoesNotExistException(const std::string& username) : SignupException("User does not exist in the server.") {}
};

/*
								DATABASEEXCEPTION
		DatabaseException is the father class of all exceptions that are involved with
		the database, whether it is queries - or errors of setting up the database itself.

		USED IN SQLITEDATABASE.CPP
*/

class DatabaseException : public ProjectException
{
public:
	DatabaseException(const std::string& message) : ProjectException(message) {}
};

/*
							DATABASECONNECTIONEXCEPTION
		DatabaseConnectionException includes all kinds of exceptions that may come
		along when dealing with the connection to the database, or its setup.
*/

class DatabaseConnectionException : public DatabaseException
{
public:
	DatabaseConnectionException(const std::string& error) : DatabaseException("Database Connection Error: " + error) {}
};

// DATABASE NOT OPEN EXCEPTION
class DatabaseNotOpenException : public DatabaseConnectionException
{
public:
	DatabaseNotOpenException() : DatabaseConnectionException("Database not open.") {}
};

// FAILED SETUP OF DATABASE EXCEPTION
class DatabaseSetupErrorException : public DatabaseConnectionException
{
public:
	DatabaseSetupErrorException() : DatabaseConnectionException("Failed to setup the database.") {}
};

// OPEN DATABASE ERROR EXCEPTION
class OpenDatabaseErrorException : public DatabaseConnectionException
{
public:
	OpenDatabaseErrorException() : DatabaseConnectionException("Failed to open the database.") {}
};

/*
							QUERYEXCEPTION
		QueryException includes all kinds of exceptions that may come
				along when dealing with database queries.
*/

class QueryException : public DatabaseException
{
public:
	QueryException(const std::string& message, const std::string& query) : DatabaseException(message + query) {}
};

// FAILED PREPARATION OF QUERY EXCEPTION
class FailedPreparationQueryException : public QueryException
{
public:
	FailedPreparationQueryException(const std::string& query) : QueryException("Failed to prepare SQL query: ", query) {}
};

// FAILED EXECUTION OF QUERY EXCEPTION
class FailedExecutionQueryException : public QueryException
{
public:
	FailedExecutionQueryException(const std::string& query) : QueryException("Failed to execute SQL query: ", query) {}
};

/*
									ROOMEXCEPTION
		RoomException is the father class of all exceptions that are involved with
							the rooms in our server.

		USED IN ROOMMANAGER.CPP
*/

class RoomException : public ProjectException
{
public:
	RoomException(const std::string& message) : ProjectException(message) {}
};

class RoomDoesNotExistException : public RoomException
{
public:
	RoomDoesNotExistException(int roomId) : RoomException("Room with id " + std::to_string(roomId) + " does not exist!") {}
};

class RoomNameInvalidException : public RoomException
{
public:
	RoomNameInvalidException() : RoomException("Room name must be within 2 to 12 characters.") {};
};

/*
									GAMEEXCEPTION
		GameException is the father class of all exceptions that are involved with
							the games in our server.

		USED IN GAME.CPP && GAMEMANAGER.CPP
*/

class GameException : public ProjectException
{
public:
	GameException(const std::string& message) : ProjectException(message) {}
};

class UserNotInGameException : public GameException
{
public:
	UserNotInGameException(const std::string& username) : GameException("User " + username + " not in the game.") {}
};

class NoMoreQuestionsException : public GameException
{
public:
	NoMoreQuestionsException() : GameException("No more questions left!") {}
};

class GameDoesNotExistException : public GameException
{
public:
	GameDoesNotExistException(unsigned int gameId) : GameException("Game with id " + std::to_string(gameId) + " does not exist.") {}
	GameDoesNotExistException() : GameException("The game does not exist!") {}
};

class InvalidQuestionCountException : public GameException
{
public:
	InvalidQuestionCountException() : GameException("Game must consist of at least 3 questions, and at maximum 20.") {}
};

class GameDidNotFinishException : public GameException
{
public:
	GameDidNotFinishException() : GameException("Game did not finish, can't get results!") {}
};

// <-- HIERARCHY -->

/*

std::exception
|
+-- ProjectException
	|
	+-- ByteException
	|   |
	|   +-- InvalidByteValueException
	|   +-- BinaryStringException
	|       |
	|       +-- InvalidBinaryStringFormatException
	|       +-- InvalidBinaryStringLengthException
	|
	+-- SocketException
	|   |
	|   +-- SocketSetupException
	|   |   |
	|   |   +-- SocketCreationErrorException
	|   |   +-- SocketBindingErrorException
	|   |   +-- SocketListeningErrorException
	|   |
	|   +-- WSAException
	|   |   |
	|   |   +-- WSASetUpErrorException
	|   |
	|   +-- SocketCommunicationException
	|   |   |
	|   |   +-- UserConnectionErrorException
	|   |   +-- SendingMessageErrorException
	|   |   +-- ReceivingMessageErrorException
	|   |
	|   +-- RequestException
	|       |
	|       +-- RequestNotRelevantException
	|
	+-- SignupException
	|   |
	|   +-- UsernameInvalidException
	|   +-- PasswordInvalidException
	|   +-- EmailInvalidException
	|   +-- UserDoesNotExistException
	|
	+-- DatabaseException
	|	|
	|	+-- DatabaseConnectionException
	|	|   |
	|	|   +-- DatabaseNotOpenException
	|	|   +-- DatabaseSetupErrorException
	|	|   +-- OpenDatabaseErrorException
	|	|
	|	+-- QueryException
	|		|
	|		+-- FailedPreparationQueryException
	|		+-- FailedExecutionQueryException
	|
	+-- RoomException
		|
		+-- RoomDoesNotExistException
		+-- RoomNameInvalidException
*/