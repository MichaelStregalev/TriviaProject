#pragma once

#include <set>
#include <string>
#include "RoomData.h"
#include "LoggedUser.h"
#include "Codes.h"

// <-- DEFINE CONSTS -->

#define ROOM_NAME_DEFAULT			"Room"
#define MAX_PLAYERS_DEFAULT			4
#define QUESTIONS_AMOUNT_DEFAULT	10
#define TIME_PER_QUESTION_DEFAULT	30

/*
							ROOM
	This class represents a single room in our trivia server,
	it will let us handle a single room - like add a user and remove 
	a user from it, and get its data.
*/


class Room
{
public:

	// EMPTY CONSTRUCTOR
	Room();

	// CONSTRUCTOR
	Room(const RoomData& data);

	// GETTERS
	std::set<LoggedUser> getAllUsers() const;
	const RoomData& getRoomData() const;

	// <-- METHODS -->
	void addUser(const LoggedUser& user);
	void removeUser(const LoggedUser& user);

private:

	// <-- FIELDS -->
	RoomData m_metadata;
	std::set<LoggedUser> m_users;
};