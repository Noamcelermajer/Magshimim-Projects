#pragma once
#include "LoginManager.h"
#include "RequestStructs.h"
#include "json.hpp"
#include "IRequestHandler.h"
#include "JsonPacketDeserializer.h"
#include "JsonPacketSerializer.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
private:
	RequestHandlerFactory* m_loginFactory;
	LoginManager& m_loginManager;

public:
	virtual bool isRequestRelevant(const Request& req);
	virtual RequestResult* handleRequest(const Request& req);
	RequestResult login(Request req);
	RequestResult signup(Request req);

	LoginRequestHandler(RequestHandlerFactory* loginFactory, LoginManager& loginManager);
	~LoginRequestHandler();

	void shutDown();
};

