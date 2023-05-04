#include "JsonPacketSerializer.h"

#define SUCCESS 100
#define FAIL 101

std::string JsonPacketSerializer::serializeResponse(const ErrorResponse& response)
{
	return response.message;
}
std::string JsonPacketSerializer::serializeResponse(const LoginResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const SignupResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const SignoutResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const CloseRoomResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const StartGameResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const LeaveRoomResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\"}";
	return finalData;
}


std::string JsonPacketSerializer::serializeResponse(const GetRoomStateResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\", \"data\": "
	+ "{\"has_game_begun\": " + std::to_string(response.hasGameBegun) + ", \"question_count\":" + std::to_string(response.questionCount) 
	+ ", \"answer_timeout: \": " + std::to_string(response.answerTimeOut) + ", \"players\": {";

	std::vector<std::string> users = response.players;
	int userNum = 0;
	for (auto i = users.begin(); i != users.end(); i++)
	{
		if (finalData[finalData.length() - 1] != '{') finalData += ", ";
		finalData += "\"" + std::to_string(userNum) + "\": " + (*i);
		userNum++;
	}
	finalData += "}}}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\", \"data\": ";
	std::string roomJson = "{";
	std::vector<RoomData> rooms = response.rooms;
	
	for (auto i = rooms.begin(); i != rooms.end(); i++)
	{
		if (roomJson[roomJson.length() - 1] != '{') roomJson += ", ";

		roomJson += "\"" + std::to_string((*i).id) + "\": {\"name\": \"" + (*i).name + "\", \"max_players\": " + std::to_string((*i).maxPlayers)
			+ ", \"question_count\": " + std::to_string((*i).questionCount) + ", \"question_timeout\": " + std::to_string((*i).timePerQuestion)
			+ ", \"is_active\": " + std::to_string((*i).isActive) + "}";
	}

	roomJson += "}}";
	finalData += roomJson;
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const HighscoreResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\", \"data\": ";
	std::map<LoggedUser, unsigned int> scores = response.highscores;
	std::string userJson = "{";

	for (auto i = scores.begin(); i != scores.end(); i++)
	{
		if (userJson[userJson.length() - 1] != '{')
			userJson += ", ";

		userJson += "\"" + (*i).first.getUsername() + "\": " + std::to_string((*i).second);
	}
	userJson += "}}";
	finalData += userJson;

	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\", \"data\": ";
	std::vector<std::string> inRoom = response.usersInRoom;
	std::string userJson = "{";
	std::string temp = "";
	int userNum = 0;

	for (auto i = inRoom.begin(); i != inRoom.end(); i++)
	{
		if (userJson[userJson.length() - 1] != '{')
			userJson += ", ";

		for (unsigned int j = 0; j < (*i).length(); j++)
		{
			if ((*i)[j] != '"')
			{
				temp += (*i)[j];
			}
		}

		userJson += "\"" + std::to_string(userNum) + "\": \"" + temp + "\"";
		temp = "";
		userNum++;
	}

	userJson += "}";
	finalData += userJson + "}";
	return finalData;
}

std::string JsonPacketSerializer::serializeResponse(const GetQuestionResponse& response)
{
	if (response.response == "get_question: success")
	{
		std::string finalData = "{\"response\": \"" + response.response + "\", \"data\": {\"question\": \"" + response.question + "\", \"answers\": {";
		std::map<unsigned int, std::string> answers = response.answers;

		for (auto i = answers.begin(); i != answers.end(); i++)
		{
			if (finalData[finalData.length() - 1] != '{') finalData += ", ";
			finalData += "\"" + std::to_string((*i).first) + "\": \"" + (*i).second + "\"";
		}

		finalData += "}}}";
		return finalData;
	}
	else
		return "{\"response\": \"" + response.response + "\"}";
}
std::string JsonPacketSerializer::serializeResponse(const GetGameResultsResponse& response)
{
	std::string finalData = "{\"response\": \"" + response.response + "\", \"data\": {";
	std::vector<PlayerResults> results = response.results;
	unsigned int index = 0;

	for (auto i = results.begin(); i != results.end(); i++)
	{
		if (finalData[finalData.length() - 1] != '{') finalData += ", ";
		finalData += "\"" + std::to_string(index) +"\": {\"username\": " + (*i).username + ", \"correct_answers\": " + std::to_string((*i).correctAnswerCount)
		+ ", \"wrong_answers\": " + std::to_string((*i).wrongAnswerCount) + ", \"average_time\": " + std::to_string((*i).averageAnswerTime) + "}";

		index++;
	}

	finalData += "}}";
	return finalData;
}
std::string JsonPacketSerializer::serializeResponse(const SubmitAnswerResponse& response)
{
	if (response.response == "submit_answer: success")
	{
		return "{\"response\": \"" + response.response + "\", \"data\": {\"correct_ans_id\": " +  std::to_string(response.correctAnswerId) + "}}";
	}
	else
	{
		return "{\"response\": \"" + response.response + "\", \"data\": {}}";
	}
}

std::string JsonPacketSerializer::serializeResponse(const GetUserStatsResponse& response)
{
	return "{\"response\": \"" + response.response + "\", \"data\": {\"highscore\": " + std::to_string(response.highScore) +
		", \"correct_ans_count\": " + std::to_string(response.correctAnswerCount) + ", \"wrong_ans_count\": " + std::to_string(response.wrongAnswerCount) +
		", \"avg_time\": " + std::to_string(response.avgTime) + "}}";
}

std::string JsonPacketSerializer::serializeResponse(const RequestResult* req)
{
	std::string type = "";
	for (unsigned int i = 0; i < req->response.length() && req->response[i] != ':'; i++)
		type += req->response[i];

	if (type == "login")
		return serializeResponse(*(LoginResponse*)req);
	else if (type == "signup")
		return serializeResponse(*(SignupResponse*)req);
	else if (type == "signout")
		return serializeResponse(*(SignoutResponse*)req);
	else if (type == "create_room")
		return serializeResponse(*(CreateRoomResponse*)req);
	else if (type == "get_highscores")
		return serializeResponse(*(HighscoreResponse*)req);
	else if (type == "join_room")
		return serializeResponse(*(JoinRoomResponse*)req);
	else if (type == "get_players_in_room")
		return serializeResponse(*(GetPlayersInRoomResponse*)req);
	else if (type == "get_rooms")
		return serializeResponse(*(GetRoomsResponse*)req);
	else if (type == "get_room_state")
		return serializeResponse(*(GetRoomStateResponse*)req);
	else if (type == "close_room")
		return serializeResponse(*(CloseRoomResponse*)req);
	else if (type == "start_game")
		return serializeResponse(*(StartGameResponse*)req);
	else if (type == "leave_room")
		return serializeResponse(*(LeaveRoomResponse*)req);
	else if (type == "get_question")
		return serializeResponse(*(GetQuestionResponse*)req);
	else if (type == "get_game_results")
		return serializeResponse(*(GetGameResultsResponse*)req);
	else if (type == "submit_answer")
		return serializeResponse(*(SubmitAnswerResponse*)req);
	else if (type == "get_user_stats")
		return serializeResponse(*(GetUserStatsResponse*)req);

	else if (type == "back_to_menu")
		return "{\"response\": \"back_to_menu: success\"}";
	else if (req->response == "err_room_shutdown")
		return "{\"response\": \"err_room_shutdown\"}";
	else if (req->response == "err_game_started")
		return "{\"response\": \"err_game_started\"}";
	else
		return "{\"response\": \"invalid\"}";
	
	delete req;
}