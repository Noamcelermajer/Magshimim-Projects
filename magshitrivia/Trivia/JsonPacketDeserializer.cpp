#include "JsonPacketDeserializer.h"
#include "json.hpp"

#define ANSWER_UP_LIM 3
#define ANSWER_DOWN_LIM 0

#define IN_LEGAL_CHARACTERS buffer[i] == '!' || buffer[i] == '?' || buffer[i] == '$' || buffer[i] == '@' || buffer[i] == '.' || buffer[i] == '_' || buffer[i] == '(' || buffer[i] == ')' || buffer[i] == ' ' || buffer[i] == '\''

LoginRequest JsonPacketDeserializer::deserializeLoginRequest(const Request& req)
{
	nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
	LoginRequest ret = LoginRequest(req.id, req.recivalTime, req.buffer, jsonData["username"], jsonData["password"]);
	
	return ret;
}
SignupRequest JsonPacketDeserializer::deserializeSignupRequest(const Request& req)
{
	nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
	SignupRequest ret = SignupRequest(req.id, req.recivalTime, req.buffer, jsonData["username"], jsonData["password"], jsonData["email"]);
	ret.id = req.id;
	ret.buffer = req.buffer;
	ret.recivalTime = req.recivalTime;

	return ret;
}
//Ver. 1

GetPlayersInRoomRequest JsonPacketDeserializer::deserializeGetPlayersRequest(const Request& req)
{
	nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
	GetPlayersInRoomRequest ret = GetPlayersInRoomRequest(req.id, req.recivalTime, req.buffer, -1);
	
	if (jsonData["room_id"] != "null")
	{
		ret = GetPlayersInRoomRequest(req.id, req.recivalTime, req.buffer, jsonData["room_id"]);
	}
	else
	{
		throw std::exception("Missing get players in room request parameters");
	}

	return ret;
}
JoinRoomRequest JsonPacketDeserializer::deserializeJoinRoomRequest(const Request& req)
{
	nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
	JoinRoomRequest ret = JoinRoomRequest(req.id, req.recivalTime, req.buffer, -1);
	if (jsonData["room_id"] != "null")
	{
		ret = JoinRoomRequest(req.id, req.recivalTime, req.buffer, jsonData["room_id"]);
	}
	else
	{
		throw std::exception("Missing join room request parameters");
	}
	
	return ret;
}
CreateRoomRequest JsonPacketDeserializer::deserializeCreateRoomRequest(const Request& req)
{
	nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
	CreateRoomRequest ret = CreateRoomRequest(req.id, req.recivalTime, req.buffer, "null", 0, 0, 0);
	if ((jsonData["room_name"] != "null" && jsonData["max_users"] != "null" && jsonData["question_count"] != "null" && jsonData["answer_timeout"] != "null") && 
		(jsonData["max_users"] > 0  && jsonData["question_count"] > 0 && jsonData["answer_timeout"] > 0))
	{
		std::string roomName = jsonData["room_name"];
		std::string buffer = roomName;

		for (size_t i = 0; i < roomName.size(); i++)
		{
			if (roomName[i] != '"' && !(isdigit(roomName[i]) || isalpha(roomName[i])) && !(IN_LEGAL_CHARACTERS))
				throw std::exception("Invalid characters in room name");
		}

		ret = CreateRoomRequest(req.id, req.recivalTime, req.buffer, jsonData["room_name"], jsonData["max_users"], jsonData["question_count"], jsonData["answer_timeout"]);		
	}

	else
	{
		throw std::exception("Missing create room request parameters");
	}

	return ret;
}
//Ver. 2

SubmitAnswerRequest JsonPacketDeserializer::deserializeSubmitAnswerRequest(const Request& req)
{
	nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
	SubmitAnswerRequest ret = SubmitAnswerRequest(req.id, req.recivalTime, req.buffer, -1);
	if (jsonData["answer_id"] != "null")
	{
		ret.answerId = jsonData["answer_id"];
		return ret;
	}

	else if (jsonData["answer_id"] > ANSWER_UP_LIM || jsonData["answer_id"] < ANSWER_DOWN_LIM)
	{
		throw std::exception("Invalid answer id submitted");
	}

	else
	{
		throw std::exception("No answer id submitted");
	}
}
//Ver. 4