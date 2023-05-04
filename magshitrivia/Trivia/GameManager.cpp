#include "GameManager.h"
#include <ctime>
#include <chrono>

#define BUFSIZE_DEFAULT 2048
#define ERROR_CONSTRUCTOR "game_over", std::vector<std::string>(), 5

GameManager::GameManager(IDatabase& database) : m_database(database)
{
	m_games = std::vector<Game*>();
}

void GameManager::deleteGame(const Game& game)
{
	for (auto i = m_games.begin(); i != m_games.end(); i++)
	{
		if ((*i)->getId() == game.getId())
		{
			delete (*i);
			m_games.erase(i);
			return;
		}
	}

	throw std::exception("Game not found");
}

void GameManager::createGame(const Room& room)
{
	std::vector<LoggedUser> users = room.getAllUsers();
	std::map<LoggedUser, GameData> userDataMap = std::map<LoggedUser, GameData>();
	std::vector<Question> questions = m_database.getQuestions(room.getRoomData().questionCount);
	
	for (auto i = users.begin(); i != users.end(); i++)
	{
		if(questions.size() > 0)
			userDataMap[*i] = GameData(questions[0], 0, 0, 0);
		else
			userDataMap[*i] = GameData(Question(ERROR_CONSTRUCTOR), 0, 0, 0);
	}
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::string startTime = std::ctime(&end_time);

	m_games.push_back(new Game(questions, userDataMap, room.getRoomData().id, room.getRoomData().timePerQuestion, startTime));
}

Game& GameManager::getGameById(unsigned int id)
{
	for (auto i = m_games.begin(); i != m_games.end(); i++)
	{
		if ((*i)->getId() == id && (*i)->getPlayers().size() > 0)
			return *(*i);
	}

	throw std::exception("Game does not exist");
}

void GameManager::addGameToDB(const Game& game) const
{
	try
	{
		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		std::string endTime = std::ctime(&end_time);

		m_database.addGame(game.getStartTime(), endTime, game.isGameFinished());
		m_database.updateUserStats(game.getPlayers());
	}

	catch (const std::exception& exp)
	{
		exp.what();
	}
}

void GameManager::updateSingleUserStats(const LoggedUser& user, const Game& game) const
{
	std::map<LoggedUser, GameData> players = game.getPlayers();
	std::map<LoggedUser, GameData> playerData = std::map<LoggedUser, GameData>();
	for (auto i = players.begin(); i != players.end(); i++)
	{
		if ((*i).first.getUsername() == user.getUsername())
		{
			playerData[user] = (*i).second;
			m_database.updateUserStats(playerData);
			return;
		}
	}
}