#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "HighscoreTable.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"


class RequestHandlerFactory
{
private:
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	HighscoreTable m_highScoreTable;
	GameManager m_gameManager;

public:
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& owner);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& owner, Room& room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, Room& room);
	GameRequestHandler* createGameRequestHandler(const LoggedUser& user, Room& room);

	RequestHandlerFactory();

	const LoginManager& getLoginManager() { return m_loginManager; }
	void adminCreateRoom(const Room& room) { m_gameManager.createGame(room); }
	Room& getRoomById(unsigned int id) { return m_roomManager.getRoomById(id); }
	Room& getRoomByName(const std::string& name) { return m_roomManager.getRoomByName(name); }
	void deleteRoomAfterGameEnd(Room** room);
};

/*
  Regarding adminCreateRoom():
  We need to create exactly one game per room. 
  The best way to do that was to create the room through the room admin's thread.
  Since none of the thread have access to the game manager,
  a function that would create the game in handlerFactory was neccessary.
*/