#include "Question.h"

Question::Question(const std::string& question, const std::vector<std::string>& possibleAnswers, unsigned int correctAnsId)
	: m_question(question), m_possibleAnswers(possibleAnswers), m_correctAnsId(correctAnsId)
{

}

std::string Question::getQuestion() const
{
	return m_question;
}

std::map<unsigned int, std::string> Question::getAnswers() const
{
	std::map<unsigned int, std::string> possibleAnswers = std::map<unsigned int, std::string>();
	
	int index = 0;
	for (auto i = m_possibleAnswers.begin(); i != m_possibleAnswers.end(); i++)
	{
		possibleAnswers[index] = (*i);
		index++;
	}

	return possibleAnswers;
}

std::string Question::getCorrectAnswer() const
{
	return m_possibleAnswers[m_correctAnsId];
}

unsigned int Question::getCorrectAnsId() const
{
	return m_correctAnsId;
}