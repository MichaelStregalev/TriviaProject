#include "Question.h"

Question::Question() : m_question(""), m_correctId(0)
{
}

Question::Question(const std::string& question, const std::vector<std::string>& wrong, const std::string& correct) : m_question(question), m_correctId(0)
{
    randomizeAnswers(wrong, correct);
}

std::string Question::getQuestion() const
{
    return m_question;
}

std::map<unsigned int, std::string> Question::getPossibleAnswers() const
{
    return m_allAnswers;
}

unsigned int Question::getCorrectId() const
{
    return m_correctId;
}

void Question::setQuestion(const std::string& question)
{
    m_question = question;
}

void Question::setAnswers(const std::vector<std::string>& wrong, const std::string& correct)
{
    randomizeAnswers(wrong, correct);
}

bool Question::operator==(const Question& other) const
{
    return m_question == other.m_question &&
           m_allAnswers == other.m_allAnswers &&
           m_correctId == other.m_correctId;
}

void Question::randomizeAnswers(const std::vector<std::string>& wrong, const std::string& correct)
{
    // Getting a vector of all the possible answers, wrong and correct.
    std::vector<std::string> allAnswers;
    allAnswers.push_back(correct);
    for (const std::string& answer : wrong)
    {
        allAnswers.push_back(answer);
    }

    // Simple shuffle using std::rand()
    std::random_shuffle(allAnswers.begin(), allAnswers.end());

    // Populating m_allAnswers, and getting the correct ID of the correct answer!
    for (int i = 0; i < allAnswers.size(); ++i)
    {
        m_allAnswers[i + 1] = allAnswers[i];
        if (allAnswers[i] == correct)
        {
            m_correctId = i + 1;
        }
    }
}
