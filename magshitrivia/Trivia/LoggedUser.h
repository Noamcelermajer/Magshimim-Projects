#pragma once
#include <string>

class LoggedUser
{
private:
	std::string m_username;

public:

	LoggedUser(std::string username);
	std::string getUsername() const;
	void setUsername(const std::string& username);
};

inline bool operator<(const LoggedUser& one, const LoggedUser& other)
{
	return one.getUsername() < other.getUsername();
}
