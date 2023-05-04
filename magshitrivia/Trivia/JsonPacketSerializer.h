#pragma once
#include "RequestStructs.h"
#include "json.hpp"

class JsonPacketSerializer
{
public:
	std::string serializeResponse(const ErrorResponse&);
	std::string serializeResponse(const LoginResponse&);
	std::string serializeResponse(const SignupResponse&);
	//Ver. 1

	std::string serializeResponse(const SignoutResponse&);
	std::string serializeResponse(const GetRoomsResponse&);
	std::string serializeResponse(const JoinRoomResponse&);
	std::string serializeResponse(const CreateRoomResponse&);
	std::string serializeResponse(const HighscoreResponse&);
	std::string serializeResponse(const GetPlayersInRoomResponse&);
	//Ver. 2

	std::string serializeResponse(const CloseRoomResponse&);
	std::string serializeResponse(const StartGameResponse&);
	std::string serializeResponse(const GetRoomStateResponse&);
	std::string serializeResponse(const LeaveRoomResponse&);
	//Ver. 3

	std::string serializeResponse(const GetQuestionResponse&);
	std::string serializeResponse(const GetGameResultsResponse&);
	std::string serializeResponse(const SubmitAnswerResponse&);
	//Ver. 4

	std::string serializeResponse(const GetUserStatsResponse&);
	std::string serializeResponse(const RequestResult*);
	//Extra
};

