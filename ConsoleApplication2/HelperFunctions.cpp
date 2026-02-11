#include "HelperFunctions.h"
#include "types.h"

#include <iostream>
#include <string>
#include <numeric>

float calculateGrade(int numerator, int denominator) {
	return (numerator / static_cast<float>(denominator)) * 100.0;
}

float calculateAvg(const std::vector<float>& grades) {
	float sum = std::accumulate(grades.begin(), grades.end(), 0.0); // accumulate function

	return sum / grades.size();
}

char letterGrade(float grade) {
	if (grade < 60) {
		return 'F';
	}
	else if (grade < 70) {
		return 'D';
	}
	else if (grade < 80) {
		return 'C';
	}
	else if (grade < 90) {
		return 'B';
	}
	else if (grade <= 100) {
		return 'A';
	}

	else {
		return 'U';
	}
}

void printColored(const std::string& text, Color color) {
	std::string code;

	switch (color) {
	case Color::Red:
		code = "\033[31m";
		break;
	case Color::Green:
		code = "\033[32m";
		break;
	case Color::Yellow:
		code = "\033[33m";
		break;
	case Color::Blue:
		code = "\033[34m";
		break;
	case Color::Purple:
		code = "\033[35m";
		break;
	case Color::Cyan:
		code = "\033[36m";
		break;

	default:
		code = "\033[0m";
	}

	std::cout << code << text << "\033[0m";
}

int sumWeightedTotal(const std::vector<Grade>& fractionals) {
	return std::accumulate(fractionals.begin(), fractionals.end(), 0,
		[](int sum, const Grade& g) {
			return sum + g.weight; // definition of what to add with accumulate
		}
	);
}

Color letterColorPick(char letter) {
	switch (letter) {
	case 'A':
	case 'B':
		return Color::Blue;
	case 'C':
	case 'D':
		return Color::Yellow;
	case 'F':
		return Color::Red;

	default:
		return Color::Default;
	}
}

int yesNoChecker(std::string& input) {
	for (char& c : input) {
		c = std::tolower(c);
	}

	if (input == "yes") {
		return 1;
	}
	else if (input == "no") {
		return 2;
	}
	else {
		return 3;
	}
}