#include "HighscoreTable.h"

HighscoreTable::HighscoreTable() : m_database(IDatabase())
{

}

std::map<LoggedUser, unsigned int> HighscoreTable::getHighscores()
{
	return m_database.getHighscores();
}