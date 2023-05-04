#pragma once
#include "LoggedUser.h"
#include "IDatabase.h"
#include <vector>
#include <list>

class LoginManager
{
private:
	std::vector<LoggedUser> m_loggedUsers;
	IDatabase m_database;

public:
	void signup(std::string username, std::string password, std::string email);
	void login(std::string username, std::string password);
	void logout(std::string username);

	void shutDown();
	bool isLogged(std::string username);

	LoginManager();
	void printLoggedUsers() { for (auto i = m_loggedUsers.begin(); i != m_loggedUsers.end(); i++) std::cout << (*i).getUsername() << std::endl; };

	IDatabase& getDatabase() { return m_database; };
};

