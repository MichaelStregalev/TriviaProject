#include <ctime>
#include <cstdlib>
#include "RoomManager.h"
#include "TriviaExceptions.h"

// CONSTRUCTOR
RoomManager::RoomManager()
{
    // Randomize the seed in order to fully randomize the room ids in the future
    std::srand(std::time(nullptr));
}

RoomStatus RoomManager::getRoomState(int roomId) const
{
    Room* room = getRoom(roomId);

    if (room != nullptr)
    {
        return (*room).getRoomData().roomStatus;
    }
    else
    {
        throw RoomDoesNotExistException(roomId);
    }
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> rooms;

    // Iterate over all rooms in the map, and add their data only if the room's status is ROOM_OPEN
    // We will also need to check that there is enough space for another player!!
    for (const auto& pair : m_rooms) 
    {
        if (pair.second.getRoomData().roomStatus == ROOM_OPEN && 
            pair.second.getAllUsers().size() < pair.second.getRoomData().maxPlayers)
        {
            rooms.push_back(pair.second.getRoomData());
        }
    }

    return rooms;
}

Room* RoomManager::getRoom(int roomId) const
{
    auto it = m_rooms.find(roomId);

    // If room exists, return a pointer to it; otherwise, return nullptr
    if (it != m_rooms.end()) 
    {
        return const_cast<Room*>(&it->second);  // Non-const pointer to Room
    }

    // If the room has not been found, throw an exception

    throw RoomDoesNotExistException(roomId);
}

int RoomManager::createRoom(const LoggedUser& user, RoomData& data)
{
    if (data.name.size() < MINIMUM_LENGTH_ROOM_NAME || data.name.size() > MAXIMUM_LENGTH_ROOM_NAME)
    {
        throw RoomNameInvalidException(data.name);
    }

    int roomId = generateUniqueRoomId();    // Generate a unique room ID
    data.id = roomId;                       // Change the ID of the room
    Room newRoom(data);                     // Create a new Room object using the provided data
    newRoom.addUser(user);                  // Add the logged user to the room

    m_rooms[roomId] = newRoom;              // Insert the new room into the map

    return roomId;
}

void RoomManager::deleteRoom(int roomId)
{
    auto it = m_rooms.find(roomId);

    // If the room exists in the map, remove it
    if (it != m_rooms.end()) 
    {
        m_rooms.erase(it);
    }
    else 
    {
        throw RoomDoesNotExistException(roomId);
    }
}

int RoomManager::generateRandomRoomId()
{
    return rand() % MAXIMUM_ROOM_ID + 1;
}

int RoomManager::generateUniqueRoomId()
{
    int roomId;

    // Generate a new random ID until we find one that doesn't already exist
    do 
    {
        roomId = generateRandomRoomId();    // Call the function to generate a random room ID

    } while (m_rooms.find(roomId) != m_rooms.end());    // Check if the roomId already exists in the map

    return roomId;
}
