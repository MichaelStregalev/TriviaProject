#pragma once

#include <map>
#include <vector>
#include <functional>
#include "LoggedUser.h"
#include "RoomData.h"
#include "Room.h"
#include "Codes.h"

// <-- DEFINE CONSTS -->

#define MINIMUM_ROOM_ID		1000
#define MAXIMUM_ROOM_ID		9999

#define MINIMUM_LENGTH_ROOM_NAME		2
#define MAXIMUM_LENGTH_ROOM_NAME		12

/*
						RoomManager
	RoomManager allows us to manage ALL rooms in our server!
*/

class RoomManager
{
public:

	// CONSTRUCTOR
	RoomManager();

	// GETTERS

	RoomStatus getRoomState(int roomId) const;			// Gets the room's state (status)
	std::vector<RoomData> getRooms() const;				// Gets all data rooms in the map (that are open!!!)
	Room* getRoom(int roomId) const;					// Get the room based on its id

	// <-- METHODS -->
	int createRoom(const LoggedUser& user, RoomData& data);		// returns the ID of the room created
	void deleteRoom(int roomId);

private:

	// <-- FIELDS -->

	std::map<int, Room> m_rooms;

	// <-- STATIC HELPER FUNCTIONS -->

	// Generates a random room id between 1000 and 9999 - 4 digit ID
	static int generateRandomRoomId();

	// Generates a unique room id that is not already in m_rooms
	int generateUniqueRoomId();
};