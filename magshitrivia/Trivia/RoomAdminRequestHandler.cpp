#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory* handlerFactory, Room& room, RoomManager& roomManager, const LoggedUser& user)
	: m_handlerFactory(handlerFactory), m_user(user), m_room(room), m_roomManager(roomManager)
{

}

bool RoomAdminRequestHandler::isRequestRelevant(const Request& req)
{
	try
	{
		nlohmann::json::parse(req.buffer);
	}

	catch (const std::exception& exp)
	{
		exp.what();
		return false;
	}

	if (req.buffer.size() != 0)
		return true; //Meaning there's an actual request.

	return false;
}

CloseRoomResponse RoomAdminRequestHandler::closeRoom(const Request& req)
{
	CloseRoomResponse crr = CloseRoomResponse("success", this, m_user.getUsername(), SUCCESS);
	m_roomManager.deleteRoom(m_room.getRoomData().name);
	return crr;
}

StartGameResponse RoomAdminRequestHandler::startGame(const Request& req)
{
	StartGameResponse sgr = StartGameResponse("invalid", nullptr, m_user.getUsername(), FAIL);
	if (m_room.getRoomData().isActive)
		sgr.response = "err_room_already_active";
	
	else
	{
		sgr.response = "success";
		sgr.requestHandler = this;
		sgr.status = SUCCESS;
		m_room.activateRoom();
	}

	return sgr;
}

GetRoomStateResponse* RoomAdminRequestHandler::getRoomState(const Request& req)
{
	RoomData data = m_room.getRoomData();
	std::vector<std::string> users = std::vector<std::string>();
	std::vector<LoggedUser> lUsers = m_room.getAllUsers();

	for (auto i = lUsers.begin(); i != lUsers.end(); i++)
		users.push_back((*i).getUsername());

	GetRoomStateResponse* grsr = new GetRoomStateResponse("success", this, m_user.getUsername(), SUCCESS, data.isActive, users, data.questionCount, data.timePerQuestion);
	return grsr;
}

RequestResult* RoomAdminRequestHandler::handleRequest(const Request& req)
{
	RequestResult* ret = new RequestResult;
	ret->requestHandler = nullptr;
	ret->response = "invalid";
	ret->username = m_user.getUsername();

	if (isRequestRelevant(req))
	{
		nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
		if (jsonData["type"] == "close_room")
		{
			*ret = closeRoom(req);
			ret->response = "close_room: " + ret->response;
			return ret;
		}

		else if (jsonData["type"] == "start_game")
		{
			*ret = startGame(req);
			ret->response = "start_game: " + ret->response;
			return ret;
		}

		else if (jsonData["type"] == "get_room_state")
		{
			delete ret;
			GetRoomStateResponse* grsr = getRoomState(req);
			grsr->response = "get_room_state: " + grsr->response;
			return grsr;
		}
	}

	return ret;
}