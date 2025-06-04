#include "Room.h"

// EMPTY CONSTRUCTOR
Room::Room() : m_metadata({0, ROOM_NAME_DEFAULT, MAX_PLAYERS_DEFAULT, QUESTIONS_AMOUNT_DEFAULT, TIME_PER_QUESTION_DEFAULT, ROOM_OPEN})
{
}

// CONSTRUCTOR WITH DATA
Room::Room(const RoomData& data) : m_metadata(data)
{
}

std::set<LoggedUser> Room::getAllUsers() const
{
	return m_users;
}

RoomData& Room::getRoomData()
{
	return m_metadata;
}

const RoomData& Room::getRoomData() const
{
	return m_metadata;
}

void Room::addUser(const LoggedUser& user)
{
	m_users.insert(user);
}

void Room::removeUser(const LoggedUser& user)
{
	m_users.erase(user);
}
