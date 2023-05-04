#pragma once
#include "Question.h"
#include <iostream>

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;

	GameData(const Question& inCurrentQuestion, unsigned int inCorrectAnswerCount, unsigned int inWrongAnswerCount, unsigned int inAverageAnswerTime)
		: currentQuestion(inCurrentQuestion), correctAnswerCount(inCorrectAnswerCount), wrongAnswerCount(inWrongAnswerCount), averageAnswerTime(inAverageAnswerTime)
	{

	}

	GameData() : currentQuestion(Question("null", std::vector<std::string>(), 0)), correctAnswerCount(0), wrongAnswerCount(0), averageAnswerTime(0)
	{

	}
};

