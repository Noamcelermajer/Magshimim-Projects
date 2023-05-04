#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(Room& room, const LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory* handlerFactory)
	: m_room(room), m_user(user), m_roomManager(roomManager), m_handlerFacroty(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const Request& req)
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
GetRoomStateResponse* RoomMemberRequestHandler::getRoomState(const Request& req)
{
	RoomData data = m_room.getRoomData();
	std::vector<std::string> users = std::vector<std::string>();
	std::vector<LoggedUser> lUsers = m_room.getAllUsers();

	for (auto i = lUsers.begin(); i != lUsers.end(); i++)
		users.push_back((*i).getUsername());

	GetRoomStateResponse* grsr = new GetRoomStateResponse("success", this, m_user.getUsername(), SUCCESS, data.isActive, users, data.questionCount, data.timePerQuestion);
	return grsr;
}
LeaveRoomResponse RoomMemberRequestHandler::leaveRoom(const Request& req)
{
	m_room.removeUser(m_user);
	return LeaveRoomResponse("success", this, m_user.getUsername(), SUCCESS);
}

RequestResult* RoomMemberRequestHandler::handleRequest(const Request& req)
{
	RequestResult* ret = new RequestResult();
	ret->requestHandler = nullptr;
	ret->response = "invalid";
	ret->username = m_user.getUsername();

	try
	{
		m_roomManager.getRoomById(m_roomId); //Will return an error if a room with that id does not exist, meaning the room's been shutdown by the admin.
	}
	catch (const std::exception& exp)
	{
		exp.what();
		ret->response = "err_room_shutdown";
		return ret;
	}
	//This part handles checking whether the room is active or not.

	if (m_room.getRoomData().isActive)
	{
		ret->response = "err_game_started";
		return ret;
	}
	//This part handles checking whether the game started or not.

	if (isRequestRelevant(req))
	{
		nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
		if (jsonData["type"] == "get_room_state")
		{
			delete ret;
			GetRoomStateResponse* grsr = getRoomState(req);
			grsr->response = "get_room_state: " + grsr->response;
			return grsr;
		}
		else if (jsonData["type"] == "leave_room")
		{
			*ret = leaveRoom(req);
			ret->response = "leave_room: " + ret->response;
			return ret;
		}
	}

	return ret;
}