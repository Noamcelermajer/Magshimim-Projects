#pragma once
#include "RoomData.h"
#include "LoggedUser.h"
#include <vector>

class Room
{
private:
	RoomData* m_metadata;
	std::vector<LoggedUser> m_users;

public:
	Room(RoomData* metadata);
	Room();
	RoomData getRoomData() const;

	void addUser(const LoggedUser& username);
	void removeUser(const LoggedUser& username);
	std::vector<LoggedUser> getAllUsers() const;

	void activateRoom() { (*m_metadata).isActive = true; };
	void deActivateRoom() {	(*m_metadata).isActive = false; };
	void deleteRoomData();
	bool isDeleted();
};

