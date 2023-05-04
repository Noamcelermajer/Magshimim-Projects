#pragma once
#include <iostream>

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int timePerQuestion;
	unsigned int questionCount;
	bool isActive;

	RoomData(unsigned int inId, const std::string& inName, unsigned int inMaxPlayers, unsigned int inTimePerQuestion, unsigned int inQuestionCount, bool inIsActive)
		: id(inId), name(inName), maxPlayers(inMaxPlayers), timePerQuestion(inTimePerQuestion), questionCount(inQuestionCount), isActive(inIsActive)
	{

	}

	RoomData()
	{
		id = 0;
		name = "null";
		maxPlayers = 0;
		timePerQuestion = 0;
		isActive = false;
	}
};