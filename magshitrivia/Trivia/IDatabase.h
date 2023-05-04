#pragma once
#include "LoggedUser.h"
#include "Question.h"
#include "sqlite3.h"
#include "GameData.h"
#include <iostream>
#include <io.h>
#include <map>
#include <string>

class IDatabase
{
private:
	sqlite3* _db;
	void createTables();

public:
	std::map<LoggedUser, unsigned int> getHighscores();
	bool doesUserExist(const std::string& username);
	std::vector<Question> getQuestions(unsigned int questionNum);
	
	std::string getPassword(const std::string& username);
	static int dataExtractPassword(void *data, int count, char **vals, char **azColName);
	static int dataExtractUserExists(void *data, int count, char **vals, char **azColName);
	static int dataExtractHighscores(void *data, int count, char **vals, char **azColName);
	static int dataExtractQuestions(void *data, int count, char **vals, char **azColName);
	static int dataCountQuestions(void *data, int count, char **vals, char **azColName);
	static int dataExtractUserStats(void *data, int count, char **vals, char **azColName);
	void addUser(const std::string& username, const std::string& password, const std::string& email);
	void addGame(const std::string& beginTime, const std::string& endTime, bool isActive);
	void updateUserStats(const std::map<LoggedUser, GameData>& players);

	static int dataExtractUserAllStats(void *data, int count, char **vals, char **azColName);
	const std::vector<unsigned int> extractAllUserStats(const std::string& username);

	void init();
	void close();
	IDatabase();
};

