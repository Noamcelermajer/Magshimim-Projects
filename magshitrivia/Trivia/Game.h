#pragma once
#include "GameData.h"
#include "LoggedUser.h"
#include "RequestStructs.h"
#include <map>
#include <mutex>

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	unsigned int m_id;
	unsigned int m_time;

	std::string m_startTime;
	std::mutex m_mtx;
	bool m_finished;
	bool m_addedToDB;

public:
	Game(const std::vector<Question>& questions, const std::map<LoggedUser, GameData>& players, unsigned int id, unsigned int time, const std::string& startTime);
	Game(const Game& other) {};

	Question getQuestionForUser(const LoggedUser& user);
	unsigned int submitAnswer(const LoggedUser& user, unsigned int answerId, const time_t& timeToAnswer);
	void removePlayer(const LoggedUser& user);

	std::vector<PlayerResults> getResults();
	unsigned int getId() const { return m_id; }
	const std::map<LoggedUser, GameData> getPlayers() const { return m_players; }
	const std::string getStartTime() const { return m_startTime; }
	const bool isGameFinished() const { return m_finished; }
	const bool userFinished(const LoggedUser& user) { return m_players[user].correctAnswerCount + m_players[user].wrongAnswerCount >= m_questions.size(); }
	const bool addedToDB() const { return m_addedToDB; }
	void addToDB() { m_addedToDB = true; }
};
