#pragma once

#include "GlobalBuildVariables.h"
#include "CalculatorNumericTypes.h"

#ifdef MULTIPRECISION
#include <gmp.h>
#else
#include <random>
#include <chrono>
#endif

class RandomNumberGenerator
{
private:
#ifdef MULTIPRECISION
	gmp_randstate_t RNGState;
#else
	mt19937 randEngine;
#endif
public:
	RandomNumberGenerator();
	void generateUniformFloat(calcFloat& result);
	void generateUniformFloat(const calcFloat& min, const calcFloat& max, calcFloat& result);
	void generateUniformInteger(const calcSignedInt& min, const calcSignedInt& max, calcSignedInt& result);
};