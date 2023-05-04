#pragma once
#include <iostream>
#include <time.h>
#include <vector>
#include "RoomData.h"
#include "HighscoreTable.h"

#define SUCCESS 100
#define FAIL 101

/*
signup:
	"signup: err_user_exist"
	"signup: err_invalid_input_email"
	"signup: err_invalid_input_username"
	"signup: err_invalid_input_password"
	"signup: success"

login:
	"login: err_user_logged_in"
	"login: err_user_doesnt_exist"
	"login: err_password_mismatch"
	"login: success"

create_room:
	"create_room: err_missing_parameters"
	"create_room: err_room_name_occupied"
	"create_room: success"

signout:
	"signout: err_user_not_logged"
	"signout: err_user_doesnt_exist"
	"signout: success"

get_highscores:
	"get_highscores: err_no_highscores"
	"get_highscores: success"

join_room:
	"join_room: err_missing_parameters"
	"join_room: err_out_of_range"
	"join_room: err_room_full"
	"join_room: err_room_active"
	"join_room: err_player_already_in_room"
	"join_room: success"

get_players_in_room:
	"get_players_in_room: err_missing_parameters"
	"get_players_in_room: err_out_of_range"
	"get_players_in_room: success"

General error:
	"invalid"
*/

#pragma region Responses
class IRequestHandler; //In order to stop circular includes.
struct RequestResult
{
	std::string response;
	IRequestHandler* requestHandler;
	std::string username;
};
struct LoginResponse : public RequestResult
{
	unsigned int status;

public:
	LoginResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct SignupResponse : public RequestResult
{
	unsigned int status;

public:
	SignupResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct ErrorResponse : public RequestResult
{
	std::string message;

public:
	ErrorResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, const std::string& inMessage) : message(inMessage)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
//Ver. 1

struct SignoutResponse : public RequestResult
{
	unsigned int status;

	SignoutResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct GetRoomsResponse : public RequestResult
{
	unsigned int status;
	std::vector<RoomData> rooms;

public:
	GetRoomsResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus, const std::vector<RoomData>& inRooms) : status(inStatus), rooms(inRooms)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct GetPlayersInRoomResponse : public RequestResult
{
	std::vector<std::string> usersInRoom;

public:
	GetPlayersInRoomResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, const std::vector<std::string>& inUsersInRoom) : usersInRoom(inUsersInRoom)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct HighscoreResponse : public RequestResult
{
	unsigned int status;
	std::map<LoggedUser, unsigned int> highscores;

public:
	HighscoreResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus, const std::map<LoggedUser, unsigned int>& inHighScores) : status(inStatus), highscores(inHighScores)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct JoinRoomResponse : public RequestResult
{
	unsigned int status;

public:
	JoinRoomResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct CreateRoomResponse : public RequestResult
{
	unsigned int status;

public:
	CreateRoomResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
//Ver. 2

struct CloseRoomResponse : public RequestResult
{
	unsigned int status;

public:
	CloseRoomResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct StartGameResponse : public RequestResult
{
	unsigned int status;
public:
	StartGameResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct LeaveRoomResponse : public RequestResult
{
	unsigned int status;
	LeaveRoomResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus) : status(inStatus)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct GetRoomStateResponse : public RequestResult
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeOut;

	GetRoomStateResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus,
		bool inHasGameBegun, const std::vector<std::string>& inPlayers, unsigned int inQuestionCount, unsigned int inAnswerTimeOut)
		: status(inStatus), hasGameBegun(inHasGameBegun), players(inPlayers), questionCount(inQuestionCount), answerTimeOut(inAnswerTimeOut)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
//Ver. 3

struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;

	PlayerResults(const std::string& inUserName, unsigned int inCorrectAnswerCount, unsigned int inWrongAnswerCount, unsigned int inAverageAnswerTime)
		: correctAnswerCount(inCorrectAnswerCount), wrongAnswerCount(inWrongAnswerCount), averageAnswerTime(inAverageAnswerTime), username(inUserName)
	{

	}
};
struct GetQuestionResponse : public RequestResult
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;

	GetQuestionResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus,
		const std::string& inQuestion, const std::map<unsigned int, std::string> inAnswers)
		: status(inStatus), question(inQuestion), answers(inAnswers)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct SubmitAnswerResponse : public RequestResult
{
	unsigned int status;
	unsigned int correctAnswerId;

	SubmitAnswerResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus, unsigned int inCorrectAnswerId)
		: status(inStatus), correctAnswerId(inCorrectAnswerId)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
struct GetGameResultsResponse : public RequestResult
{
	unsigned int status;
	std::vector<PlayerResults> results;

	GetGameResultsResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUsername, unsigned int inStatus, const std::vector<PlayerResults>& inResults)
		: status(inStatus), results(inResults)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUsername;
	}
};
//Ver. 4

struct GetUserStatsResponse : public RequestResult
{
	unsigned int highScore;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int avgTime;

	GetUserStatsResponse(const std::string& inResponse, IRequestHandler* inHandler, const std::string& inUserName, unsigned int inCorrectAnswerCount, unsigned int inWrongAnswerCount, unsigned int inAverageAnswerTime)
		: correctAnswerCount(inCorrectAnswerCount), wrongAnswerCount(inWrongAnswerCount), avgTime(inAverageAnswerTime)
	{
		response = inResponse;
		requestHandler = inHandler;
		username = inUserName;
	}
};
//Extra
#pragma endregion

#pragma region Requests
struct Request
{
	unsigned int id;
	time_t recivalTime;
	std::string buffer;
};
struct LoginRequest : public Request
{
	std::string username;
	std::string password;

public:
	LoginRequest(unsigned int inId, const time_t& inRecivalTime, const std::string& inBuffer, const std::string& inUsername, const std::string& inPassword)
		: username(inUsername), password(inPassword)
	{
		id = inId;
		recivalTime = inRecivalTime;
		buffer = inBuffer;
	}
};
struct SignupRequest : public Request
{
	std::string username;
	std::string password;
	std::string email;

public:
	SignupRequest(unsigned int inId, const time_t& inRecivalTime, const std::string& inBuffer, const std::string& inUsername, const std::string& inPassword, const std::string& inEmail)
		: username(inUsername), password(inPassword), email(inEmail)
	{
		id = inId;
		recivalTime = inRecivalTime;
		buffer = inBuffer;
	}
};
//Ver. 1

struct GetPlayersInRoomRequest : public Request
{
	unsigned int roomId;

public:
	GetPlayersInRoomRequest(unsigned int inId, const time_t& inRecivalTime, const std::string& inBuffer, unsigned int inRoomId) : roomId(inRoomId)
	{
		id = inId;
		recivalTime = inRecivalTime;
		buffer = inBuffer;
	}
};
struct JoinRoomRequest : public Request
{
	unsigned int roomId;

public:
	JoinRoomRequest(unsigned int inId, const time_t& inRecivalTime, const std::string& inBuffer, unsigned int inRoomId) : roomId(inRoomId)
	{
		id = inId;
		recivalTime = inRecivalTime;
		buffer = inBuffer;
	}
};
struct CreateRoomRequest : public Request
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;

public:
	CreateRoomRequest(unsigned int inId, const time_t& inRecivalTime, const std::string& inBuffer, const std::string& inRoomName,
		unsigned int inMaxUsers, unsigned int inQuestionCount, unsigned int inAnswerTimeout) : roomName(inRoomName), maxUsers(inMaxUsers),
		questionCount(inQuestionCount), answerTimeout(inAnswerTimeout)
	{
		id = inId;
		recivalTime = inRecivalTime;
		buffer = inBuffer;
	}
};
//Ver. 2

struct SubmitAnswerRequest: public Request
{
	unsigned int answerId;

	SubmitAnswerRequest(unsigned int inId, const time_t& inRecivalTime, const std::string& inBuffer, unsigned int inAnswerId) : answerId(inAnswerId)
	{
		id = inId;
		recivalTime = inRecivalTime;
		buffer = inBuffer;

	}
};
//Ver. 4
#pragma endregion