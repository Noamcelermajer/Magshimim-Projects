#pragma once
#include "RoomManager.h"
#include "IRequestHandler.h"
#include "json.hpp"
#include "RequestStructs.h"
#include "JsonPacketDeserializer.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
private:
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory* m_handlerFacroty;
	unsigned int m_roomId;

public:
	virtual bool isRequestRelevant(const Request& req);
	virtual RequestResult* handleRequest(const Request& req);

	LeaveRoomResponse leaveRoom(const Request&);
	GetRoomStateResponse* getRoomState(const Request&);

	RoomMemberRequestHandler(Room& room, const LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory* handlerFactory);
};

