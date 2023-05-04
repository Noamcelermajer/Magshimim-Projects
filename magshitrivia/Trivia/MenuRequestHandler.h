#pragma once
#include "RoomManager.h"
#include "HighscoreTable.h"
#include "RequestStructs.h"
#include "IRequestHandler.h"
#include "json.hpp"
#include "LoginManager.h"
#include "JsonPacketDeserializer.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
private:
	LoggedUser m_user;
	RoomManager& m_roomManager;
	HighscoreTable& m_highScoreTable;
	RequestHandlerFactory* m_handlerFactory;
	LoginManager& m_loginManager;
	IDatabase& m_database;
	JsonPacketDeserializer m_deserializer;

public:

	MenuRequestHandler(const LoggedUser& user, RoomManager& roomManager, HighscoreTable& highScoreTable, RequestHandlerFactory* handlerFactory, LoginManager& loginManager);
	virtual bool isRequestRelevant(const Request& req);
	SignoutResponse signOut(const Request& req);
	GetPlayersInRoomResponse* getPlayersInRoom(const Request& req);
	HighscoreResponse* getHighScores(const Request& req);
	JoinRoomResponse joinRoom(const Request& req);
	CreateRoomResponse createRoom(const Request& req);
	virtual RequestResult* handleRequest(const Request& req);
	GetRoomsResponse* getRooms(const Request& req);
	GetUserStatsResponse* getUserStats(const Request& req);
};

