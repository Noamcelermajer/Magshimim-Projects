#pragma once
#include "Room.h"
#include <map>

class RoomManager
{
private:
	std::map<int, Room> m_rooms;

public:
	void createRoom(const LoggedUser& creator, RoomData* metadata);
	void deleteRoom(const std::string& roomName);
	bool getRoomState(int id);
	std::vector<Room> getRooms();

	Room& getRoomById(unsigned int id);
	Room& getRoomByName(const std::string& name);
	void deleteUserFromRoom(const std::string& username);
	RoomManager();
};

