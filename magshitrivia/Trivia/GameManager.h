#pragma once
#include "Game.h"
#include "IDatabase.h"
#include "Room.h"

class GameManager
{
	IDatabase& m_database;
	std::vector<Game*> m_games;

public:
	GameManager(IDatabase& database);
	void createGame(const Room& room);
	void deleteGame(const Game& game);

	Game& getGameById(unsigned int id);
	void addGameToDB(const Game& game) const;
	void updateSingleUserStats(const LoggedUser& user, const Game& game) const;
};

