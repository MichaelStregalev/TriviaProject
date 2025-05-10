#pragma once

#include <string>
#include <vector>

/*
							Question
			Question is a basic structure that will allow us
	to store information about all the questions in the trivia project.
*/

struct Question
{
	std::string question; // The question itSelf
	std::vector<std::string> wrong; // 3 Wrong answers
	std::string correct; // 1 Correct answer

} typedef Question;