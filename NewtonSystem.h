#pragma once
#include <vector>
#include <stdexcept>

#include "Globals.h"
#include "GaussJordanElim.h"



#ifndef NEWTONSYSTEM_H

namespace ld{
	bool isCloseEnough(std::vector<long double>* x, std::vector<long double>* old_x, long double eps);

	int NewtonSystem(uint16_t  n,
		std::vector<long double>* x,
		std::function<long double(uint16_t i, uint16_t n, std::vector<long double>* x)> f,
		std::function<void(uint16_t i, uint16_t n, std::vector<long double>* x, std::vector<long double>* out)> df,
		uint16_t                mit,
		long double             eps,
		uint16_t*                it);
}

namespace iv{
	using namespace interval_arithmetic;
	bool isCloseEnough(std::vector<Interval<long double>>* x, std::vector<Interval<long double>>* old_x, Interval<long double> eps);

	//return 0 on succes, 2 when matrix is bad
	int NewtonSystem(uint16_t  n,
		std::vector<Interval<long double>>* x,
		std::function<Interval<long double>(uint16_t i, uint16_t n, std::vector<Interval<long double>>* x)> f,
		std::function<void(uint16_t i, uint16_t n, std::vector<Interval<long double>>* x, std::vector<Interval<long double>>* out)> df,
		uint16_t                          mit,
		Interval<long double>             eps,
		uint16_t*                          it);
}


#endif // !NEWTONSYSTEM_H
