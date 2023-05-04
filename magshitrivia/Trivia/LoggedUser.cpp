#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username) : m_username(username)
{

}

std::string LoggedUser::getUsername() const
{
	return m_username;
}

void LoggedUser::setUsername(const std::string& username)
{
	m_username = username;
}