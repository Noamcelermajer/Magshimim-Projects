#pragma once
#include "GameManager.h"
#include "IRequestHandler.h"
#include "json.hpp"
#include "JsonPacketDeserializer.h"

class RequestHandlerFactory;
class GameRequestHandler : public IRequestHandler
{
	GameManager& m_gameManager;
	Game& m_game;
	LoggedUser m_user;
	RequestHandlerFactory* m_handlerFactory;
	bool m_gotQuestion;
	
	time_t m_timeToAnswer;

public:
	GameRequestHandler(GameManager& gameManager, unsigned int gameId, const LoggedUser& user, RequestHandlerFactory* handlerFactory);
	virtual bool isRequestRelevant(const Request& req);
	virtual RequestResult* handleRequest(const Request& req);

	GetQuestionResponse* getQuestion(const Request& req);
	GetGameResultsResponse* getGameResults(const Request& req);
	SubmitAnswerResponse* submitAnswer(const Request& req);
};

