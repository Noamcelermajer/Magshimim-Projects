#pragma once
#include "IRequestHandler.h"
#include "RequestStructs.h"
#include "RequestHandlerFactory.h"
#include <iostream>
#include <WS2tcpip.h>
#include <algorithm>
#include <map>
#include <thread>
#pragma comment (lib, "ws2_32.lib")

#define WINVERSION MAKEWORD(2, 2);
#define PORT 54000

class Communicator
{
private:
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory m_handlerFactory;
	JsonPacketSerializer m_serializer;
	JsonPacketDeserializer m_deserializer;

public:
	void bindAndListen();
	void handleRequests();
	void startThreadForNewClient(const SOCKET& user);

	Communicator();
	std::thread makeThread(const SOCKET& user);

	std::string requestSequencer(const SOCKET& user, const size_t threadNum);
	void handleResponse(const RequestResult* resp, const SOCKET& user, const Room* room);
	void switchStates(const RequestResult& resp, const Request& req, const SOCKET& user, std::string* username, std::string* state, Room** room);
	void endConnection(const std::string& info, const SOCKET& user);
	void handleInGameCrash(const SOCKET& user, const std::string& username, Room** room);
};
