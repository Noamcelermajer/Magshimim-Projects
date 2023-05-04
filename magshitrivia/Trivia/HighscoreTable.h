#pragma once
#include "IDatabase.h"

class HighscoreTable
{
private:
	class IDatabase m_database;

public:
	HighscoreTable();
	
	std::map<LoggedUser, unsigned int> getHighscores();
};

