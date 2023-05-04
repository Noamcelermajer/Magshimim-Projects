#include "MenuRequestHandler.h"

#define STATS_SIZE 4
#define CORRECT_ANS_INDEX 0
#define WRONG_ANS_INDEX 1
#define AVG_TIME_INDEX 2
#define HIGHSCORE_INDEX 3

MenuRequestHandler::MenuRequestHandler(const LoggedUser& user, RoomManager& roomManager, HighscoreTable& highScoreTable, RequestHandlerFactory* handlerFactory, LoginManager& loginManager)
	: m_user(user), m_roomManager(roomManager), m_highScoreTable(highScoreTable), m_handlerFactory(handlerFactory), m_loginManager(loginManager),
	m_database(m_loginManager.getDatabase())
{
}
bool MenuRequestHandler::isRequestRelevant(const Request& req)
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
CreateRoomResponse MenuRequestHandler::createRoom(const Request& req)
{
	CreateRoomResponse result = CreateRoomResponse("invalid", nullptr, m_user.getUsername(), FAIL);
	try
	{
		CreateRoomRequest crr = m_deserializer.deserializeCreateRoomRequest(req);
		RoomData* data = new RoomData(0, crr.roomName, crr.maxUsers, crr.answerTimeout, crr.questionCount, false); //ID not yet set, the room manager will give it the id.
		m_roomManager.createRoom(m_user, data);
		result.response = "success";
		result.requestHandler = this;
		result.status = SUCCESS;
	}

	catch (const std::exception& exp)
	{
		std::string resp = exp.what();
		if (resp == "Missing create room request parameters")
		{
			result.response = "err_missing_parameters";
		}
		else if (resp == "Room name occupied")
		{
			result.response = "err_room_name_occupied";
		}
		else if (resp == "Invalid characters in room name")
		{
			result.response = "err_invalid_chars_in_room_name";
		}
	}

	return result;
}
SignoutResponse MenuRequestHandler::signOut(const Request& req)
{
	SignoutResponse result = SignoutResponse("invalid", nullptr, m_user.getUsername(), FAIL);
	nlohmann::json jsonData = nlohmann::json::parse(req.buffer);

	std::string username = jsonData["username"].dump();
	if (username != "null" && username == m_user.getUsername())
	{
		std::string temp = "";
		for (unsigned int i = 0; i < username.size(); i++)
			if (username[i] != '"')
				temp += username[i];
		username = temp;

		try
		{	
			m_loginManager.logout(username);
			result.response = "success";
			result.requestHandler = this;
			result.status = SUCCESS;

			m_roomManager.deleteUserFromRoom(username);
		}

		catch (const std::exception& exp)
		{
			std::string err = exp.what();

			if (err == "User not logged")
			{
				result.response = "err_user_not_logged";
			}

			else if (err == "User does not exist")
			{
				result.response = "err_user_doesnt_exist";
			}
		}
	}

	else
	{
		result.response = "err_username_mismatch";
	}

	return result;
}
HighscoreResponse* MenuRequestHandler::getHighScores(const Request& req)
{
	HighscoreResponse* result = new HighscoreResponse("invalid", nullptr, m_user.getUsername(), FAIL, std::map<LoggedUser, unsigned int>());
	std::map<LoggedUser, unsigned int> scores = m_highScoreTable.getHighscores();
	
	if (scores.size() > 0)
	{
		result->response = "success";
		result->requestHandler = this;
		result->status = SUCCESS;
		result->highscores = scores;
	}

	else
	{
		result->response = "err_no_highscores";
	}

	return result;
}
JoinRoomResponse MenuRequestHandler::joinRoom(const Request& req)
{
	JoinRoomResponse result = JoinRoomResponse("invalid", nullptr, m_user.getUsername(), FAIL);

	try
	{
		JoinRoomRequest jrr = m_deserializer.deserializeJoinRoomRequest(req);

		std::vector<Room> rooms = m_roomManager.getRooms();
		std::vector<LoggedUser> usersInRoom = std::vector<LoggedUser>();
		for (auto i = rooms.begin(); i != rooms.end(); i++)
		{
			usersInRoom = (*i).getAllUsers();
			for (auto j = usersInRoom.begin(); j != usersInRoom.end(); j++)
			{
				if ((*j).getUsername() == m_user.getUsername())
					throw std::exception("Player already in room");
			}
		}

		m_roomManager.getRoomById(jrr.roomId).addUser(m_user);
		result.status = SUCCESS;
		result.requestHandler = this;
		result.response = "success";
	}

	catch (const std::exception& exp)
	{
		std::string resp = exp.what();

		if (resp == "Missing join room request parameters")
		{
			result.response = "err_missing_parameters";
		}

		else if (resp == "Id out of range")
		{
			result.response = "err_out_of_range";
		}

		else if (resp == "Room full")
		{
			result.response = "err_room_full";
		}

		else if (resp == "Room active")
		{
			result.response = "err_room_active";
		}

		else if (resp == "Player already in room")
		{
			result.response = "err_player_already_in_room";
		}
	}

	return result;
}
GetPlayersInRoomResponse* MenuRequestHandler::getPlayersInRoom(const Request& req)
{
	GetPlayersInRoomResponse* result = new GetPlayersInRoomResponse("invalid", nullptr, m_user.getUsername(), std::vector<std::string>());
	
	try
	{
		GetPlayersInRoomRequest gpirr = m_deserializer.deserializeGetPlayersRequest(req);
		std::vector<LoggedUser> users = m_roomManager.getRoomById(gpirr.roomId).getAllUsers();
		std::vector<std::string> usernames = std::vector<std::string>();

		for (auto i = users.begin(); i != users.end(); i++)
		{
			usernames.push_back((*i).getUsername());
		}

		result->response = "success";
		result->requestHandler = this;
		result->usersInRoom = usernames;
	}

	catch (const std::exception& exp)
	{
		std::string resp = exp.what();
		
		if (resp == "Missing get players in room request parameters")
		{
			result->response = "err_missing_parameters";
		}

		else if (resp == "Id out of range")
		{
			result->response = "err_out_of_range";
		}
	}

	return result;
}
GetRoomsResponse* MenuRequestHandler::getRooms(const Request& req)
{
	GetRoomsResponse* result = new GetRoomsResponse("invalid", nullptr, m_user.getUsername(), FAIL, std::vector<RoomData>());
	std::vector<RoomData> ret = std::vector<RoomData>();
	std::vector<Room> rooms = m_roomManager.getRooms();

	for (auto i = rooms.begin(); i != rooms.end(); i++)
	{
		ret.push_back((*i).getRoomData());
	}

	result->response = "success";
	result->requestHandler = this;
	result->status = SUCCESS;
	result->rooms = ret;
	
	return result;
}

GetUserStatsResponse* MenuRequestHandler::getUserStats(const Request& req)
{
	GetUserStatsResponse* result = new GetUserStatsResponse("success", this, m_user.getUsername(), 0, 0, 0);
	std::vector<unsigned int> stats = m_database.extractAllUserStats(m_user.getUsername());

	result->avgTime = stats[AVG_TIME_INDEX];
	result->correctAnswerCount = stats[CORRECT_ANS_INDEX];
	result->wrongAnswerCount = stats[WRONG_ANS_INDEX];
	result->highScore = stats[HIGHSCORE_INDEX];

	return result;
}

RequestResult* MenuRequestHandler::handleRequest(const Request& req)
{
	RequestResult* ret = new RequestResult();
	ret->requestHandler = nullptr;
	ret->response = "invalid";
	ret->username = m_user.getUsername();

	if (isRequestRelevant(req))
	{
		nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
		if (jsonData["type"] == "signout")
		{
			*ret = signOut(req);
			ret->response = "signout: " + ret->response;
			return ret;
		}
		else if (jsonData["type"] == "create_room")
		{
			*ret = createRoom(req);
			ret->response = "create_room: " + ret->response;
			return ret;
		}
		else if (jsonData["type"] == "join_room")
		{
			*ret = joinRoom(req);
			ret->response = "join_room: " + ret->response;
			return ret;
		}
		else if (jsonData["type"] == "get_highscores")
		{
			delete ret;
			HighscoreResponse* hsrp = getHighScores(req);
			hsrp->response = "get_highscores: " + hsrp->response;
			return hsrp;
		}
		else if (jsonData["type"] == "get_players_in_room")
		{
			delete ret;
			GetPlayersInRoomResponse* gpirr = getPlayersInRoom(req);
			gpirr->response = "get_players_in_room: " + gpirr->response;
			return gpirr;
		}
		else if (jsonData["type"] == "get_rooms")
		{
			delete ret;
			GetRoomsResponse* grr = getRooms(req);
			grr->response = "get_rooms: " + grr->response;
			return grr;
		}
		else if(jsonData["type"] == "get_user_stats")
		{
			delete ret;
			GetUserStatsResponse* gusr = getUserStats(req);
			gusr->response = "get_user_stats: " + gusr->response;
			return gusr;
		}
		else
		{
			return ret;
		}
	}
	return ret;
}