#pragma once
#include "IRequestHandler.h"
#include "RequestStructs.h"
#include "RoomManager.h"
#include "json.hpp"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	Room& m_room;
	RoomManager& m_roomManager;
	LoggedUser m_user;
	RequestHandlerFactory* m_handlerFactory;

public:
	virtual bool isRequestRelevant(const Request& req);
	virtual RequestResult* handleRequest(const Request& req);

	GetRoomStateResponse* getRoomState(const Request& req);
	StartGameResponse startGame(const Request& req);
	CloseRoomResponse closeRoom(const Request& req);

	RoomAdminRequestHandler(RequestHandlerFactory* handlerFactory, Room& room, RoomManager& roomManager, const LoggedUser& user);
};