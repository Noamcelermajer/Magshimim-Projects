#pragma once
#include <iostream>
#include <vector>
#include <map>

class Question
{
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	unsigned int m_correctAnsId;

public:
	std::string getQuestion() const;
	std::map<unsigned int, std::string> getAnswers() const;
	std::string getCorrectAnswer() const;
	unsigned int getCorrectAnsId() const;

	Question(const std::string& question, const std::vector<std::string>& possibleAnswers, unsigned int correctAnsId);
};

