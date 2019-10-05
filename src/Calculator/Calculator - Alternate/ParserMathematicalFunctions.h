#pragma once

#include <vector>

#include "CalcObj.h"
#include "ParsingSettings.h"
#include "BuiltInConstants.h"

namespace ParserMathematicalFunctions
{
	calcFloat convertAngle(ParsingSettings::angleMode srcUnits, ParsingSettings::angleMode destUnits, calcFloat value);

	void deltaVector(std::vector<calcFloat>& vec);

	calcFloat average(const std::vector<calcFloat>& vec);
	calcFloat stddev(const std::vector<calcFloat>& vec);
	calcFloat variance(const std::vector<calcFloat>& vec);
	calcFloat sum(const std::vector<calcFloat>& vec);
	calcFloat product(const std::vector<calcFloat>& vec);
	calcFloat max(const std::vector<calcFloat>& vec);
	calcFloat min(const std::vector<calcFloat>& vec);

	calcSignedInt binomialCoefficient(unsigned degree, unsigned index);

	class SingleVariableFunction
	{
	public:
		virtual calcFloat evaluate(calcFloat value) const = 0;
		virtual ~SingleVariableFunction() = default;
	};

	bool findBisectionSolution(const SingleVariableFunction* evaluationFunction,
		calcFloat lBound,
		calcFloat hBound,
		calcFloat& result);

	void findMultipleBisectionSolutions(const SingleVariableFunction* evaluationFunction,
		calcFloat lBound,
		calcFloat hBound,
		std::vector<calcFloat>& solutions,
		unsigned numIntervals);

	void primeFactorization(std::vector<calcFloat>& results, calcSignedInt number);
	bool isPrime(calcSignedInt number);
	calcSignedInt leastCommonMultiple(calcSignedInt firstNum, calcSignedInt secondNum);
	calcSignedInt greatestCommonDenominator(calcSignedInt firstNum, calcSignedInt secondNum);
	calcFloat factorial(calcFloat value);
	calcSignedInt factorial(unsigned value);
	calcFloat scaleNum(calcFloat oldMin, calcFloat oldMax, calcFloat newMin, calcFloat newMax, calcFloat value);
}