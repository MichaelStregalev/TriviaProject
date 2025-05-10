#pragma once

#include <string>
#include <vector>

struct Question
{
	std::string question; //The question itSelf
	std::vector<std::string> wrong; //3 Wrong answers
	std::string correct; //1 Correct answer
} typedef Question;