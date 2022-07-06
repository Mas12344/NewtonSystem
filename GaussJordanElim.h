#pragma once
#include <vector>

#include "Globals.h"
#ifndef GAUSSJORDANELIM_H

namespace ld{
	void swapRows(std::vector<std::vector<long double>>* x, int first, int second);

	void divideRow(std::vector<std::vector<long double>>* x, int index, long double val);

	// second += first * mult;
	void addMultipliedRow(std::vector<std::vector<long double>>* x, int first, int second, long double mult);

	int vectoredGaussJordan(std::vector<std::vector<long double>>* x, std::vector<long double>* res);

}

namespace iv{
	using namespace interval_arithmetic;
	void swapRows(std::vector<std::vector<Interval<long double>>>* x, int first, int second);

	void divideRow(std::vector<std::vector<Interval<long double>>>* x, int index, Interval<long double> val);

	// second += first * mult;
	void addMultipliedRow(std::vector<std::vector<Interval<long double>>>* x, int first, int second, Interval<long double> mult);

	int vectoredGaussJordan(std::vector<std::vector<Interval<long double>>>* x, std::vector<Interval<long double>>* res);


}

#endif // !GAUSSJORDANELIM_H
