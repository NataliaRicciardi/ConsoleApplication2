#pragma once

#include "types.h"

#include <vector>
#include <string>

float calculateGrade(int numerator, int denominator);
float calculateAvg(const std::vector<float>& grades);
char letterGrade(float grade);
void printColored(const std::string& text, Color color);
int sumWeightedTotal(const std::vector<Grade>& fractionals);
Color letterColorPick(char letter);
int yesNoChecker(std::string& input);
