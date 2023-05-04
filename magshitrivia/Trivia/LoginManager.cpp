#include "LoginManager.h"

#define IN_LEGAL_CHARACTERS buffer[i] == '!' || buffer[i] == '?' || buffer[i] == '$' || buffer[i] == '@' || buffer[i] == '.' || buffer[i] == '_' || buffer[i] == '(' || buffer[i] == ')'

LoginManager::LoginManager() : m_loggedUsers(std::vector<LoggedUser>()), m_database(IDatabase())
{
	
}

void LoginManager::signup(std::string username, std::string password, std::string email)
{
	std::string temp = "";
	std::string buffer = username;

	//Gets rid of the " brackets added by json and makes sure
	//all characters in the username are in the whitelist.

	/*=======================
	= Whitelist includes:   =
	= 1) Digits             =
	= 2) English letters    =
	= 3) . @ ! ? $ _ ( )    =
	=========================*/

	for (size_t i = 0; i < username.size(); i++)
	{
		if (username[i] != '"' && (isdigit(username[i]) || isalpha(username[i])))
			temp += username[i];

		else if (username[i] != '"' && !(isdigit(username[i]) || isalpha(username[i])) && !(IN_LEGAL_CHARACTERS))
			throw std::exception("Invalid characters in username");
	}
	username = temp;

	//At this point it's safe to try and find out if the user already exists. (SQL statement won't be injected)
	if (m_database.doesUserExist(username))
		throw std::exception("User already exists");

	//Now we can check the password and email to make sure they are both SQL friendly.
	buffer = password;
	for (size_t i = 0; i < password.size(); i++)
	{
		if (password[i] != '"' && !(isdigit(password[i]) || isalpha(password[i])) && !(IN_LEGAL_CHARACTERS))
			throw std::exception("Invalid characters in password");
	}

	buffer = email;
	for (size_t i = 0; i < email.size(); i++)
	{
		if (email[i] != '"' && !(isdigit(email[i]) || isalpha(email[i])) && !(IN_LEGAL_CHARACTERS))
			throw std::exception("Invalid characters in email");
	}

	//By now the user is all clear for signing up.
	m_database.addUser(username, password, email);
}

void LoginManager::login(std::string username, std::string password)
{
	if (m_database.doesUserExist(username))
	{
		if (!isLogged(username))
		{
			std::string correctPassword = m_database.getPassword(username);
			if (correctPassword == password)
			{
				m_loggedUsers.push_back(LoggedUser(username));
				return;
			}
			else
			{
				throw std::exception("Password mismatch");
			}
		}
		else
		{
			throw std::exception("User already logged in");
		}
	}
	else
	{
		throw std::exception("User does not exist");
	}
}

void LoginManager::logout(std::string username)
{
	if (m_database.doesUserExist(username))
	{
		for (auto i = m_loggedUsers.begin(); i != m_loggedUsers.end(); i++)
		{
			if ((*i).getUsername() == username)
			{
				m_loggedUsers.erase(i);
				return;
			}
		}
		throw std::exception("User not logged");
	}
	else
	{
		throw std::exception("User does not exist");
	}
}

void LoginManager::shutDown()
{
	m_database.close();
}

bool LoginManager::isLogged(std::string username)
{
	for (auto i = m_loggedUsers.begin(); i != m_loggedUsers.end(); i++)
		if ((*i).getUsername() == username)
			return true;

	return false;
}