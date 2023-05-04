#pragma once
#include "RequestStructs.h"

class JsonPacketDeserializer
{
public:
	LoginRequest deserializeLoginRequest(const Request& req);
	SignupRequest deserializeSignupRequest(const Request& req); 
	//Ver. 1
	
	GetPlayersInRoomRequest deserializeGetPlayersRequest(const Request& req);
	JoinRoomRequest deserializeJoinRoomRequest(const Request& req);
	CreateRoomRequest deserializeCreateRoomRequest(const Request& req);
	//Ver. 2

	SubmitAnswerRequest deserializeSubmitAnswerRequest(const Request& req);
	//Ver. 4
};

