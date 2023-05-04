#include "Game.h"
#include <string>

#define BUFSIZE_DEFAULT 2048
#define TWO_TO_GET_AVERAGE 2
#define GAME_OVER_ANS 5

Game::Game(const std::vector<Question>& questions, const std::map<LoggedUser, GameData>& players, unsigned int id, unsigned int inTime, const std::string& startTime)
	: m_questions(questions), m_players(players), m_id(id), m_time(inTime), m_finished(false), m_startTime(startTime), m_addedToDB(false)
{
}

Question Game::getQuestionForUser(const LoggedUser& user)
{
	if(!m_finished) return m_players[user].currentQuestion; 
	else return Question("game_over", std::vector<std::string>(), 0);
}

std::vector<PlayerResults> Game::getResults()
{
	std::vector<PlayerResults> ret = std::vector<PlayerResults>();

	for (auto i = m_players.begin(); i != m_players.end(); i++)
	{
		ret.push_back(PlayerResults((*i).first.getUsername(), (*i).second.correctAnswerCount, (*i).second.wrongAnswerCount, (*i).second.averageAnswerTime));
	}

	return ret;
}

unsigned int Game::submitAnswer(const LoggedUser& user, unsigned int answerId, const time_t& timeToAnswer)
{
	unsigned int correctAnsId = getQuestionForUser(user).getCorrectAnsId();
	if (answerId == correctAnsId && timeToAnswer <= m_time)
	{
		m_players[user].correctAnswerCount++;
	}
	else
	{
		m_players[user].wrongAnswerCount++;
	}
	
	if (m_players[user].averageAnswerTime != 0)
	{
		m_players[user].averageAnswerTime = (m_players[user].averageAnswerTime + timeToAnswer) / TWO_TO_GET_AVERAGE;
	}
	else
	{
		m_players[user].averageAnswerTime += timeToAnswer;
	}
	
	if (m_players[user].correctAnswerCount + m_players[user].wrongAnswerCount < m_questions.size())
		m_players[user].currentQuestion = m_questions[m_players[user].correctAnswerCount + m_players[user].wrongAnswerCount];

	else
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_finished = true;

		for (auto i = m_players.begin(); i != m_players.end(); i++)
		{
			if ((*i).second.wrongAnswerCount + (*i).second.correctAnswerCount < m_questions.size())
				m_finished = false;
		}
		if(!m_finished)
			m_players[user].currentQuestion = Question("game_over", std::vector<std::string>(), 0);

		lock.unlock();
	}
	
	return correctAnsId;
}

void Game::removePlayer(const LoggedUser& user)
{
	for (auto i = m_players.begin(); i != m_players.end(); i++)
	{
		if ((*i).first.getUsername() == user.getUsername())
		{
			m_players.erase(i);
			return;
		}
	}

	throw std::exception("Player not found");
}