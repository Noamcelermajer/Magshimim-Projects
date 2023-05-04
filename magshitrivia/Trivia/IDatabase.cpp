#include "IDatabase.h"
#include <time.h>

#define CREATE_TABLE_USER "CREATE TABLE USERS(USERNAME TEXT PRIMARY KEY NOT NULL, PASSWORD TEXT NOT NULL, EMAIL TEXT NOT NULL, HIGHSCORE INTEGER NOT NULL, CORRECT_ANS INTEGER NOT NULL, WRONG_ANS INTEGER NOT NULL, AVG_TIME INTEGER NOT NULL);"
#define CREATE_TABLE_QUESTION "CREATE TABLE QUESTIONS(QUESTION_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, QUESTION TEXT NOT NULL, CORRECT_ANS TEXT NOT NULL, ANS2 TEXT NOT NULL, ANS3 TEXT NOT NULL, ANS4 TEXT NOT NULL);"
#define CREATE_TABLE_GAME "CREATE TABLE GAMES(GAME_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, STATUS TEXT NOT NULL, START_TIME TEXT NOT NULL, END_TIME TEXT NOT NULL);"

#define POSSIBLE_ANSWERS_SIZE 4
#define CORRECT_ANS_INDEX 0
#define ANS2_INDEX 1
#define ANS3_INDEX 2
#define ANS4_INDEX 3
#define MAX_ANSWER_ID 3

#define STATS_SIZE 4
#define CORRECT_ANS_INDEX 0
#define WRONG_ANS_INDEX 1
#define AVG_TIME_INDEX 2
#define HIGHSCORE_INDEX 3

#define TWO_TO_GET_AVG 2

const std::vector<unsigned int> IDatabase::extractAllUserStats(const std::string& username)
{
	std::vector<unsigned int> stats = std::vector<unsigned int>(STATS_SIZE);
	std::string request = "SELECT * FROM USERS WHERE USERNAME LIKE " + username;
	char* errmsg = 0;
	
	int res = sqlite3_exec(_db, request.c_str(), dataExtractUserAllStats, &stats, &errmsg);
	if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";
	return stats;
}

int IDatabase::dataExtractUserAllStats(void *data, int count, char **vals, char **azColName)
{
	std::vector<unsigned int>* stats = (std::vector<unsigned int>*)data;
	std::string curColName = "";

	for (int i = 0; i < count; i++)
	{
		curColName = azColName[i];
		if (curColName == "CORRECT_ANS")
			(*stats)[CORRECT_ANS_INDEX] = atoi(vals[i]);
		else if(curColName == "WRONG_ANS")
			(*stats)[WRONG_ANS_INDEX] = atoi(vals[i]);
		else if(curColName == "AVG_TIME")
			(*stats)[AVG_TIME_INDEX] = atoi(vals[i]);
		else if(curColName == "HIGHSCORE")
			(*stats)[HIGHSCORE_INDEX] = atoi(vals[i]);
	}

	return 0;
}

const std::string noBrackets(const std::string& toPurge)
{
	std::string temp = "";
	for (size_t i = 0; i < toPurge.size(); i++)
		if (toPurge[i] != '"')
			temp += toPurge[i];

	return temp;
}

void IDatabase::updateUserStats(const std::map<LoggedUser, GameData>& players)
{
	std::string request = "";
	char* errmsg = 0;
	std::map<LoggedUser, unsigned int> highScores = getHighscores();
	int res = 0;
	std::vector<unsigned int> curUserStats = std::vector<unsigned int>();
	unsigned int avgTimeFinal = 0;

	for (auto i = players.begin(); i != players.end(); i++)
	{
		for (auto k = highScores.begin(); k != highScores.end(); k++)
		{
			if (noBrackets((*k).first.getUsername()) == noBrackets((*i).first.getUsername()) && (*i).second.correctAnswerCount > (*k).second)
			{
				request = "UPDATE USERS SET HIGHSCORE = " + std::to_string((*i).second.correctAnswerCount) + " WHERE USERNAME LIKE " + (*i).first.getUsername();
				res = sqlite3_exec(_db, request.c_str(), nullptr, nullptr, &errmsg);
				if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";
			}
		}

		request = "SELECT * FROM USERS WHERE USERNAME LIKE " + (*i).first.getUsername();
		res = sqlite3_exec(_db, request.c_str(), dataExtractUserStats, &curUserStats, &errmsg);
		if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";

		if (curUserStats[AVG_TIME_INDEX] != 0) avgTimeFinal = ((*i).second.averageAnswerTime + curUserStats[AVG_TIME_INDEX]) / TWO_TO_GET_AVG;
		else avgTimeFinal = (*i).second.averageAnswerTime;

		request = "UPDATE USERS SET CORRECT_ANS = " + std::to_string((*i).second.correctAnswerCount + curUserStats[CORRECT_ANS_INDEX])
				+ ", WRONG_ANS = " + std::to_string((*i).second.wrongAnswerCount + curUserStats[WRONG_ANS_INDEX])
				+ ", AVG_TIME = " + std::to_string(avgTimeFinal) + " WHERE USERNAME LIKE " + (*i).first.getUsername();
		res = sqlite3_exec(_db, request.c_str(), nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";
	}
}

int IDatabase::dataExtractUserStats(void *data, int count, char **vals, char **azColName)
{
	std::vector<unsigned int>* stats = (std::vector<unsigned int>*)data;
	stats->clear();
	std::string curColName = "";

	for (int i = 0; i < count; i++)
	{
		curColName = azColName[i];
		if (curColName == "CORRECT_ANS" || curColName == "WRONG_ANS" || curColName == "AVG_TIME")
			stats->push_back(atoi(vals[i]));
	}

	return 0;
}

void IDatabase::addGame(const std::string& beginTime, const std::string& endTime, bool isActive)
{
	std::string active = "";

	if (isActive)
		active = "Finished";
	else
		active = "Unfinished";

	std::string request = "INSERT INTO GAMES(STATUS, START_TIME, END_TIME) VALUES(\"" + active + "\", \"" + beginTime + "\", \"" + endTime + "\");";
	char* errmsg = 0;

	int res = sqlite3_exec(_db, request.c_str(), nullptr, nullptr, &errmsg);
	if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";
}

void IDatabase::createTables()
{
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, CREATE_TABLE_USER, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cout << "USERS: Error creating table." << std::endl;

	res = sqlite3_exec(_db, CREATE_TABLE_QUESTION, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cout << "QUESTIONS: Error creating table." << std::endl;

	res = sqlite3_exec(_db, CREATE_TABLE_GAME, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cout << "PICTURES: Error creating table." << std::endl;
}

std::map<LoggedUser, unsigned> IDatabase::getHighscores()
{
	std::map<std::string, unsigned int> scores = std::map<std::string, unsigned int>();
	std::map<LoggedUser, unsigned int> scoresRet = std::map<LoggedUser, unsigned int>();
	std::string request = "SELECT USERNAME, HIGHSCORE FROM USERS;";
	LoggedUser cur = LoggedUser("null");
	char* errmsg = 0;

	int res = sqlite3_exec(_db, request.c_str(), dataExtractHighscores, &scores, &errmsg);
	if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";

	for (auto i = scores.begin(); i != scores.end(); i++)
	{
		try
		{	
			cur = LoggedUser((*i).first);
			scoresRet[cur] = (*i).second;
		}
		catch (const std::exception& exp)
		{
			std::cout << exp.what() << std::endl;
		}
	}

	return scoresRet;
}

int IDatabase::dataExtractHighscores(void *data, int count, char **vals, char **azColName)
{
	std::map<std::string, unsigned int>* scores = (std::map<std::string, unsigned int>*)data;
	char* username = 0;
	int score = -1;
	std::string curColName = "";

	for (int i = 0; i < count; i++)
	{
		curColName = azColName[i];
		if (curColName == "USERNAME")
			username = vals[i];
		else if (curColName == "HIGHSCORE")
			score = abs(atoi(vals[i]));

		if (username != 0 && score != -1)
		{
			(*scores)[username] = score;
			username = 0;
			score = -1;
		}
	}

	return 0;
}

bool IDatabase::doesUserExist(const std::string& username)
{
	std::string temp = "";
	for (unsigned int i = 0; i < username.size(); i++)
		if (username[i] != '"')
			temp += username[i];
	
	bool exists = false;
	std::string request = "SELECT * FROM USERS WHERE USERNAME LIKE ";
	request += '\"' + temp + "\";";
	char* errmsg = 0;

	int res = sqlite3_exec(_db, request.c_str(), dataExtractUserExists, &exists, &errmsg);
	if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";
	return exists;
}

std::vector<Question> IDatabase::getQuestions(unsigned int questionNum)
{
	srand(time(NULL));
	unsigned int qidb = 0;
	std::string request = "SELECT * FROM QUESTIONS;";
	char* errmsg = 0;
	std::vector<Question> questions = std::vector<Question>();

	int res = sqlite3_exec(_db, request.c_str(), dataCountQuestions, &qidb, &errmsg);
	if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";
	if (qidb < questionNum)
		questionNum = qidb;
	
	std::vector<unsigned int> chosen = std::vector<unsigned int>();
	unsigned int questionId = 0;
	bool newNum = false;
		
	while (chosen.size() < questionNum)
	{
		newNum = false;
		while (!newNum)
		{
			newNum = true;
			questionId = rand() % questionNum + 1;
			for (auto i = chosen.begin(); i != chosen.end(); i++)
			{
				if ((*i) == questionId)
					newNum = false;
			}
		}

		request = "SELECT * FROM QUESTIONS WHERE QUESTION_ID == " + std::to_string(questionId);
		res = sqlite3_exec(_db, request.c_str(), dataExtractQuestions, &questions, &errmsg);
		if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";

		chosen.push_back(questionId);
	}

	return questions;
}

int IDatabase::dataExtractQuestions(void *data, int count, char **vals, char **azColName)
{
	std::vector<Question>* questions = (std::vector<Question>*)data;
	std::vector<std::string> possibleAnswers = std::vector<std::string>(POSSIBLE_ANSWERS_SIZE);
	std::string question = "";
	std::string curColName = "";
	std::vector<unsigned int> answerOrder = std::vector<unsigned int>();
	unsigned int answerId = 0;
	bool newNum = false;
	bool dataCollected = false;
	
	while (answerOrder.size() < POSSIBLE_ANSWERS_SIZE)
	{
		newNum = false;
		while (!newNum)
		{
			newNum = true;
			answerId = rand() % POSSIBLE_ANSWERS_SIZE;
			for (auto i = answerOrder.begin(); i != answerOrder.end(); i++)
			{
				if ((*i) == answerId)
					newNum = false;
			}
		}
		answerOrder.push_back(answerId);
	}

	for (int i = 0; i < count; i++)
	{
		curColName = azColName[i];
		if (curColName == "QUESTION")
			question = vals[i];

		else if (curColName == "ANS2")
			possibleAnswers[answerOrder[ANS2_INDEX]] = vals[i];
		else if (curColName == "ANS3")
			possibleAnswers[answerOrder[ANS3_INDEX]] = vals[i];
		else if (curColName == "ANS4")
		{
			possibleAnswers[answerOrder[ANS4_INDEX]] = vals[i];
			dataCollected = true;
		}

		else if (curColName == "CORRECT_ANS")
			possibleAnswers[answerOrder[CORRECT_ANS_INDEX]] = vals[i];

		if (dataCollected)
		{
			questions->push_back(Question(question, possibleAnswers, answerOrder[CORRECT_ANS_INDEX]));
			possibleAnswers.clear();
			question = "";
			dataCollected = false;
		}
	}

	return 0;
}

int IDatabase::dataCountQuestions(void *data, int count, char **vals, char **azColName)
{
	unsigned int* qidb = (unsigned int*)data;
	(*qidb)++; //For each time this function is called, another question is being loaded -> QuestionsInDataBase++.
	return 0;
}

std::string IDatabase::getPassword(const std::string& username)
{
	std::string password = "";
	std::string request = "SELECT PASSWORD FROM USERS WHERE USERNAME LIKE ";
	request += '\"' + username + "\";";
	char* errmsg = 0;

	int res = sqlite3_exec(_db, request.c_str(), dataExtractPassword, &password, &errmsg);
	return password;
}

int IDatabase::dataExtractPassword(void *data, int count, char **vals, char **azColName)
{
	std::string* password = (std::string*)data;
	*password = vals[0];
	return 0;
}

int IDatabase::dataExtractUserExists(void *data, int count, char **vals, char **azColName)
{
	bool* exists = (bool*)data;
	if (vals == nullptr)
		*exists = false;
	else
		*exists = true;

	return 0;
}

void IDatabase::addUser(const std::string& username, const std::string& password, const std::string& email)
{
	std::string request = "INSERT INTO USERS(USERNAME, PASSWORD, EMAIL, HIGHSCORE, CORRECT_ANS, WRONG_ANS, AVG_TIME) VALUES('" + username + "', '" + password + "', '" + email + "', 0, 0, 0, 0);";
	char* errmsg = 0;
	int res = sqlite3_exec(_db, request.c_str(), nullptr, nullptr, &errmsg);
	if (res != SQLITE_OK) std::cout << request << ": Bad SQL Statement!\n";
}

IDatabase::IDatabase() : _db(nullptr)
{
	init();
}
void IDatabase::init()
{
	std::string dbFileName = "TriviaDB.sqlite";
	int doesFileExist = _access(dbFileName.c_str(), 0);
	
	if(doesFileExist != 0) //DB file does not exist.
	{
		int res = sqlite3_open(dbFileName.c_str(), &_db);

		if (res != SQLITE_OK)
		{
			_db = nullptr;
			std::cout << "Failed to open DB" << std::endl;
		}

		else
		{
			createTables();
			std::cout << "Database Created. SQL-API Functioning." << std::endl;
		}
	}

	else //DB file exists.
	{
		int res = sqlite3_open(dbFileName.c_str(), &_db);
	}
}
void IDatabase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
}