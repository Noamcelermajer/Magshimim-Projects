#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(GameManager& gameManager, unsigned int gameId, const LoggedUser& user, RequestHandlerFactory* handlerFactory)
	: m_gameManager(gameManager), m_user(user), m_handlerFactory(handlerFactory), m_game(m_gameManager.getGameById(gameId)), m_gotQuestion(false)
{
	
}

bool GameRequestHandler::isRequestRelevant(const Request& req)
{
	try
	{
		nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
		if (jsonData["type"] == "submit_answer" || jsonData["type"] == "get_game_results" || jsonData["type"] == "get_question" || jsonData["type"] == "back_to_menu")
			return true;
		return false;
	}

	catch (const std::exception& exp)
	{
		exp.what();
		return false;
	}

	return false;
}
RequestResult* GameRequestHandler::handleRequest(const Request& req)
{
	RequestResult* ret = new RequestResult();
	ret->response = "invalid";
	ret->requestHandler = nullptr;
	ret->username = m_user.getUsername();

	if (isRequestRelevant(req))
	{
		nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
		
		if (jsonData["type"] == "get_question")
		{
			if (m_game.userFinished(m_user))
			{
				ret->response = "get_question: err_no_questions_left";
				return ret;
			}
			else if (!m_gotQuestion && !m_game.userFinished(m_user))
			{
				m_timeToAnswer = req.recivalTime;
				m_gotQuestion = true;
				delete ret;
				GetQuestionResponse* gqr = getQuestion(req);
				gqr->response = "get_question: " + gqr->response;
				return gqr;
			}
			else
			{
				ret->response = "get_question: err_already_got_question";
				return ret;
			}
		}

		else if (jsonData["type"] == "get_game_results")
		{
			delete ret;
			GetGameResultsResponse* ggrr = getGameResults(req);
			ggrr->response = "get_game_results: " + ggrr->response;
			return ggrr;
		}

		else if (jsonData["type"] == "submit_answer")
		{
			if (m_game.isGameFinished())
			{
				ret->response = "submit_answer: err_all_answered";
				if (!m_game.addedToDB())
				{
					m_gameManager.addGameToDB(m_game);
					m_game.addToDB();
				}
				return ret;
			}
			else if (!m_game.isGameFinished() && m_gotQuestion)
			{
				m_timeToAnswer = req.recivalTime - m_timeToAnswer;
				m_gotQuestion = false;
				delete ret;
				SubmitAnswerResponse* sar = submitAnswer(req);
				sar->response = "submit_answer: " + sar->response;
				return sar;
			}
			else
			{
				ret->response = "sumbit_answer: err_didnt_get_question";
				return ret;
			}
		}

		else if (jsonData["type"] == "back_to_menu")
		{
			ret->response = "back_to_menu: success";
			ret->username = m_user.getUsername();
			ret->requestHandler = this;

			m_gameManager.updateSingleUserStats(m_user, m_game);
			if (m_game.getPlayers().size() <= 1) //Last player in the room should shut it off and register it to the DB!
			{
				m_gameManager.addGameToDB(m_game);
				m_game.addToDB();
				m_gameManager.deleteGame(m_game);
			}
			
			else
			{
				m_game.removePlayer(m_user);
			}

			return ret;
		}

		else
			return ret;
	}

	return ret;
}

GetQuestionResponse* GameRequestHandler::getQuestion(const Request& req)
{
	GetQuestionResponse* gqr = new GetQuestionResponse("invalid", nullptr, m_user.getUsername(), FAIL, "", std::map<unsigned int, std::string>());
	Question question = m_game.getQuestionForUser(m_user);
	
	gqr->answers = question.getAnswers();
	gqr->question = question.getQuestion();
	gqr->requestHandler = this;
	gqr->status = SUCCESS;
	gqr->response = "success";

	return gqr;
}
GetGameResultsResponse* GameRequestHandler::getGameResults(const Request& req)
{
	GetGameResultsResponse* ggrr = new GetGameResultsResponse("invalid", nullptr, m_user.getUsername(), FAIL, std::vector<PlayerResults>());
	
	ggrr->requestHandler = this;
	ggrr->response = "success";
	ggrr->results = m_game.getResults();
	ggrr->status = SUCCESS;

	return ggrr;
}
SubmitAnswerResponse* GameRequestHandler::submitAnswer(const Request& req)
{
	SubmitAnswerResponse* ret = new SubmitAnswerResponse("invalid", nullptr, m_user.getUsername(), FAIL, 0);
	JsonPacketDeserializer deserializer = JsonPacketDeserializer();
	unsigned int correctAnsId = 0;

	try
	{
		SubmitAnswerRequest sar = deserializer.deserializeSubmitAnswerRequest(req);
		correctAnsId = m_game.submitAnswer(m_user, sar.answerId, m_timeToAnswer);
		ret->correctAnswerId = correctAnsId;
		ret->response = "success";
		ret->requestHandler = this;
		ret->status = SUCCESS;
		return ret;
	}

	catch (const std::exception& exp)
	{
		std::string resp = exp.what();
		if (resp == "Invalid answer id submitted")
		{
			ret->response = "err_invalid_id";
		}
		else if (resp == "No answer id submitted")
		{
			ret->response = "err_no_id";
		}
		return ret;
	}
	return ret;
}