#include "HelperFunctions.h"
#include "types.h"

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <format>
#include <fstream>


int main()
{
	std::string name;
	std::vector<Grade> fractionals;
	std::vector<float> grades;
	std::vector<float> weighted;
	std::vector<GradeResult> results;
	std::string outputString;

	printColored("----------- Grade Calculator -----------\n ", Color::Default); // title
	printColored("What's your name?\n Enter here: ", Color::Green);
	std::cin >> name;

	outputString = std::format("\nHello, Student {}!\nEnter your grades, and pick the denominator (e.g. 90/100).\n", name);

	printColored(outputString, Color::Green);
	printColored("Type \"q\" to finish. \n", Color::Red);

	std::string weightedInput;
	bool weightedGrades = false;
	
	bool contLoop = true;
	do {
		printColored("\nDo you want to use weighted grades? (yes or no): ", Color::Green);
		std::cin >> weightedInput;

		int yesOrNo = yesNoChecker(weightedInput);
		
		if (yesOrNo == 1) {
			weightedGrades = true;  
			contLoop = false;
		}
		else if (yesOrNo == 2) {
			weightedGrades = false;
			contLoop = false;
		}
	} while (contLoop);
	

	std::string numerator;
	std::string denominator;
	int counter = 1;
	bool error = false;
	contLoop = true;
	while (contLoop) {
		do {
			error = false;
			
			std::cout << "\nGrade " << counter << "\nNumerator: ";
			std::cin >> numerator;

			if (numerator == "q") {
				contLoop = false;
				break;
			}

			std::cout << "Out of: ";
			std::cin >> denominator;

			if (denominator == "q") {
				contLoop = false;
				break;
			}

			int numeratorI;
			int denominatorI;
			size_t posN;
			size_t posD;
			try {
				numeratorI = std::stoi(numerator, &posN);
				denominatorI = std::stoi(denominator, &posD);

				if (posN != numerator.length() || posD != denominator.length()) {
					error = true;
					printColored("Invalid Input. Extra characters.\n", Color::Red);
					continue;
				}
			}
			catch (const std::invalid_argument&) {
				printColored("Invalid Input. Non-integer input.\n", Color::Red);
				error = true;
				continue;
			}
			catch (const std::out_of_range&) {
				printColored("Invalid Input. Number out of range.\n", Color::Red);
				error = true;
				continue;
			}

			if (numeratorI < 0 || denominatorI <= 0 || numeratorI > denominatorI) {
				printColored("Invalid Input. Inadmissible fractional values.\n", Color::Red);
				error = true;
				continue;
			}

			
			int weightedI = 0;
			if (weightedGrades) {
				bool nestedError = false;
				do {
					nestedError = false;


					printColored("Percent of total grade: ", Color::Blue);
					std::cin >> weightedInput;

					if (!weightedInput.empty()) {
						if (weightedInput.back() == '%') weightedInput.pop_back();
					}
					else {
						nestedError = true;
						printColored("Invalid Input. Cannot be empty.\n", Color::Red);
						continue;
					}

					
					size_t posW;
					try {
						weightedI = std::stoi(weightedInput, &posW);

						if (posW != weightedInput.length()) {
							printColored("Invalid Input. Extra characters.\n", Color::Red);
							nestedError = true;
							continue;
						}
					}
					catch (const std::invalid_argument&) {
						printColored("Invalid Input. Non-integer input.\n", Color::Red);
						nestedError = true;
						continue;
					}
					catch (const std::out_of_range&) {
						printColored("Invalid Input. Number out of range.\n", Color::Red);
						nestedError = true;
						continue;
					}
					if (weightedI > 100 || weightedI < 0) {
						printColored("Invalid Input. Number cannot be greater than 100% or less than 0%.\n", Color::Red);
						nestedError = true;
						continue;
					}

				} while (nestedError);
			}
			
			if (!error) {
				fractionals.push_back({ numeratorI, denominatorI, weightedI });
			}
		} while (error);
		
		counter++;
	}
	
	if (fractionals.empty()) {
		printColored("\nNo grades to calculate :)\n", Color::Yellow);
		return 0;
	}

	int weightsSum = sumWeightedTotal(fractionals);

	if (weightsSum > 100 || weightsSum < 0) {
		printColored("\nError. Weights totaled to over 100% or under 0%.\nCannot calculate weighted grade.", Color::Red);
		weightedGrades = false;
	}

	int index = 1;
	for (const auto& frac : fractionals) { // automatically deduced type + reference so no copying
		float grade = calculateGrade(frac.numerator, frac.denominator);

		outputString = std::format("\nCalculated {}: {:.2f}%.\n", index, grade);

		printColored(outputString, Color::Cyan);
		
		grades.push_back(grade);

		GradeResult result;
		result.base = frac;
		result.percent = grade;

		if (weightedGrades) {
			float weightedGrade = grade * (frac.weight / 100.0);

			outputString = std::format("\n  Calculated Weighted {}: {:.2f}% earned out of {}%\n", index, weightedGrade, frac.weight);

			printColored(outputString, Color::Default);
			
			weighted.push_back(weightedGrade);

			result.weightedPercent = weightedGrade;
		}
		else {
			result.weightedPercent = 0;
		}

		results.push_back(result);

		index++;
	}

	float avg = calculateAvg(grades);

	outputString = std::format("\nThe average of your grades is {:.2f}%.\n", avg);
	printColored(outputString, Color::Cyan);

	char avgLetter = letterGrade(avg);

	if (avgLetter == 'U') {
		printColored("Something went wrong. Your grade is above 100% or below 0%", Color::Red);
		return 1;
	}

	Color letterColor = letterColorPick(avgLetter);

	printColored("Your average letter grade is ", Color::Green);
	printColored({ avgLetter }, letterColor);

	// weighted grades
	
	float sumWeightedGrades;
	if (weightedGrades) {
		sumWeightedGrades = std::accumulate(weighted.begin(), weighted.end(), 0.0);
		
		outputString = std::format("\nYou've achieved {:.2f}% of your final course grade from the {}% of assessments completed so far.\n", sumWeightedGrades, weightsSum);
		printColored(outputString, Color::Cyan);
	}

	std::string saveFile;
	contLoop = true;
	bool save = false;
	while (contLoop) {
		printColored("\nDo you want to save your grades to a file? (yes or no): ", Color::Green);
		std::cin >> saveFile;

		int saveAns = yesNoChecker(saveFile);

		if (saveAns == 1) {
			contLoop = false;
			save = true;
		}
		else if (saveAns == 2) {
			contLoop = false;
			save = false;
		}
	}

	if (save) {
		std::ofstream file(std::format("data/Student{}Grades.txt", name)); // open or make file
		
		if (file.is_open()) {
			file << "------------------------Grade Calculator Result-------------------------\n";
			
			file << std::format("\nName: {}\n", name);
			
			index = 0;
			for (const auto& g : results) {
				file << std::format("\nGrade {}: \n", index + 1);
				file << std::format("{} / {} = {:.2f}\n", g.base.numerator, g.base.denominator, g.percent);
				if (weightedGrades) {
					file << std::format("Grade weight: {}%\n", g.base.weight);
					file << std::format("Weight achieved: {:.2f}%\n", g.weightedPercent);
				}
			}
			
			file << "\n------------------------------------------------------------------------";

			file << std::format("\nAverage Grade: {:.2f}\n", avg);
			file << std::format("Average Letter: {}\n", avgLetter);

			if (weightedGrades) {
				file << std::format("Weighted Sum: {}% out of {}%\n", sumWeightedGrades, weightsSum);
			}
			
		}

		file.close();

		printColored("File Created Successfully!", Color::Green);
	}
	else {
		printColored("\nEnd of program.", Color::Green);
	}
}
