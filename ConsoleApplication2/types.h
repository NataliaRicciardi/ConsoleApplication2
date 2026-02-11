#pragma once

enum class Color {
	Red,
	Green,
	Yellow,
	Blue,
	Purple,
	Cyan,
	Default
};

struct Grade {
	int numerator;
	int denominator;
	int weight;
};

struct GradeResult {
	Grade base;
	float percent;
	float weightedPercent;
};