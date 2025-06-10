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

void Question::addAnswers(const std::vector<std::string>& answers)
{
    // Preserving the correct answer
    std::string correct = m_allAnswers[m_correctId];

    std::vector<std::string> allAnswers;

    for (const auto& pair : m_allAnswers)
    {
        allAnswers.push_back(pair.second);
    }

    for (const auto& answer : answers)
    {
        allAnswers.push_back(answer);
    }

    std::random_shuffle(allAnswers.begin(), allAnswers.end());

    // Making the new map
    std::map<unsigned int, std::string> newMapAnswers;

    for (int i = 0; i < allAnswers.size(); i++)
    {
        newMapAnswers[i + 1] = allAnswers[i];
        if (allAnswers[i] == correct)
        {
            m_correctId = i + 1;
        }
    }

    m_allAnswers = std::move(newMapAnswers);
}

void Question::addAnswer(const std::string& answer, bool isCorrect)
{
    std::string correct;
    if (!isCorrect)
    {
        // Preserving the correct answer
        correct = m_allAnswers[m_correctId];
    }
    else
    {
        correct = answer;
    }

    std::vector<std::string> allAnswers;

    for (const auto& pair : m_allAnswers)
    {
        allAnswers.push_back(pair.second);
    }

    allAnswers.push_back(answer);

    std::random_shuffle(allAnswers.begin(), allAnswers.end());

    // Making the new map
    std::map<unsigned int, std::string> newMapAnswers;

    for (int i = 0; i < allAnswers.size(); i++)
    {
        newMapAnswers[i + 1] = allAnswers[i];
        if (allAnswers[i] == correct)
        {
            m_correctId = i + 1;
        }
    }

    m_allAnswers = std::move(newMapAnswers);
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
