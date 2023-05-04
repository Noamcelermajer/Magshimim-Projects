#include "Room.h"

Room::Room()
{
	m_metadata = new RoomData();
	*m_metadata = { 0, "null", 0, 0, 0, false };
	m_users = std::vector<LoggedUser>();
}

Room::Room(RoomData* metadata) : m_metadata(metadata)
{
	m_users = std::vector<LoggedUser>();
}

bool Room::isDeleted()
{
	if (m_metadata == nullptr) return true;
	else return false;
}

void Room::addUser(const LoggedUser& user)
{
	for (auto i = m_users.begin(); i != m_users.end(); i++)
	{
		if ((*i).getUsername() == user.getUsername())
		{
			throw std::exception("Player already in room");
		}
	}

	if (m_users.size() < (*m_metadata).maxPlayers && !(*m_metadata).isActive)
		m_users.push_back(user);

	else if (m_users.size() >= (*m_metadata).maxPlayers)
		throw std::exception("Room full");

	else if ((*m_metadata).isActive)
		throw std::exception("Room active");
}

void Room::removeUser(const LoggedUser& user)
{
	for (auto i = m_users.begin(); i != m_users.end(); i++)
	{
		if ((*i).getUsername() == user.getUsername())
		{
			m_users.erase(i);
			return;
		}
	}
}

std::vector<LoggedUser> Room::getAllUsers() const
{
	return m_users;
}

RoomData Room::getRoomData() const
{
	return *m_metadata;
}

void Room::deleteRoomData()
{
	delete m_metadata;
}