#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>

/*
							Question
			Question is a basic class that will allow us
	to store information about all the questions in the trivia project.
*/

class Question
{
public:

	// EMPTY CONSTRUCTOR
	Question();

	// CONSTRUCTOR
	Question(const std::string& question, const std::vector<std::string>& wrong, const std::string& correct);

	// GETTERS
	std::string getQuestion() const;
	std::map<unsigned int, std::string> getPossibleAnswers() const;
	unsigned int getCorrectId() const;

	// SETTERS
	void setQuestion(const std::string& question);
	void setAnswers(const std::vector<std::string>& wrong, const std::string& correct);
	void addAnswers(const std::vector<std::string>& answers);
	void addAnswer(const std::string& answer, bool isCorrect);

private:

	// <-- FIELDS -->
	std::string m_question;
	std::map<unsigned int, std::string> m_allAnswers;
	unsigned int m_correctId;

	// <-- PRIVATE METHODS -->
	void randomizeAnswers(const std::vector<std::string>& wrong, const std::string& correct);
};