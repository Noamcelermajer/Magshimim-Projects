#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory() : m_loginManager(LoginManager()), m_gameManager(GameManager(m_loginManager.getDatabase()))
{

}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(this, m_loginManager);
}
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& owner)
{
	return new MenuRequestHandler(owner, m_roomManager, m_highScoreTable, this, m_loginManager);
}
RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& owner, Room& room)
{
	return new RoomAdminRequestHandler(this, room, m_roomManager, owner);
}
RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, Room& room)
{
	return new RoomMemberRequestHandler(room, user, m_roomManager, this);
}
GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user, Room& room)
{
	try
	{
		GameRequestHandler* ret =  new GameRequestHandler(m_gameManager, room.getRoomData().id, user, this);
		return ret;
	}
	catch (const std::exception& exp)
	{
		exp.what();
		return nullptr;
	}
}

void RequestHandlerFactory::deleteRoomAfterGameEnd(Room** room)
{
	if ((**room).getAllUsers().size() == 0)
	{
		m_roomManager.deleteRoom((*room)->getRoomData().name);
		*room = nullptr;
	}
}