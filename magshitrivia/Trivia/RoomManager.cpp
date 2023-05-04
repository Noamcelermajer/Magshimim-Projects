#include "RoomManager.h"

RoomManager::RoomManager() : m_rooms(std::map<int, Room>())
{
}

void RoomManager::createRoom(const LoggedUser& creator, RoomData* metadata)
{
	for (auto i = m_rooms.begin(); i != m_rooms.end(); i++)
	{
		if ((*i).second.getRoomData().name == metadata->name)
		{
			throw std::exception("Room name occupied");
		}
	}

	Room newRoom = Room(metadata);
	newRoom.addUser(creator);
	
	bool created = false;
	int id = 0;
	
	while (!created)
	{
		if(m_rooms[id].getRoomData().name == "null" && m_rooms[id].getRoomData().maxPlayers == 0)
		{ 
			metadata->id = id;
			m_rooms[id] = newRoom;
			created = true;
		}

		else id++;
	}
}

void RoomManager::deleteRoom(const std::string& roomName)
{
	int index = 0;
	for (auto i = m_rooms.begin(); i != m_rooms.end(); i++)
	{
		if ((*i).second.getRoomData().name == roomName)
		{
			(*i).second.deleteRoomData();
			m_rooms.erase(i);
			return;
		}
		index++;
	}
}

bool RoomManager::getRoomState(int id)
{
	try
	{	
		return m_rooms[id].getRoomData().isActive;
	}
	
	catch (const std::exception& exp)
	{
		exp.what();
		return false;
	}

	return false;
}

std::vector<Room> RoomManager::getRooms()
{
	std::vector<Room> rooms = std::vector<Room>();

	for (auto i = m_rooms.begin(); i != m_rooms.end(); i++)
		rooms.push_back((*i).second);

	return rooms;
}

Room& RoomManager::getRoomById(unsigned int id)
{
	Room& ret = m_rooms[id];
	if (ret.isDeleted())
	{
		throw std::exception("Room erased");
	}
	if (ret.getRoomData().name == "null" && ret.getRoomData().maxPlayers == 0)
	{
		m_rooms.erase(id);
		throw std::exception("Id out of range");
	}

	return ret;
}

Room& RoomManager::getRoomByName(const std::string& name)
{
	std::string temp = "";
	for (unsigned int i = 0; i < name.length(); i++)
		if (name[i] != '"') temp += name[i];

	for (auto i = m_rooms.begin(); i != m_rooms.end(); i++)
	{
		if ((*i).second.getRoomData().name == temp)
		{
			return (*i).second;
		}
	}

	throw std::exception("Room not found"); //Never happens - function is only ever called when the switching states, when the room surely exists.
}

void RoomManager::deleteUserFromRoom(const std::string& username)
{
	for (auto i = m_rooms.begin(); i != m_rooms.end(); i++)
	{
		std::vector<LoggedUser> users = (*i).second.getAllUsers();
		for (auto j = users.begin(); j != users.end(); j++)
		{
			if ((*j).getUsername() == username)
			{
				(*i).second.removeUser(LoggedUser(username));
				return;
			}
		}
	}
}