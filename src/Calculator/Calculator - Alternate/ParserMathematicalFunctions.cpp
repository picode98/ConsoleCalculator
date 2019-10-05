#include "ParsingSettings.h"
#include "ParserMathematicalFunctions.h"

using namespace std;

calcFloat ParserMathematicalFunctions::convertAngle(ParsingSettings::angleMode srcUnits, ParsingSettings::angleMode destUnits, calcFloat value)
{
	calcFloat result = value;

	switch(srcUnits)
	{
	case ParsingSettings::radians:
		switch (destUnits)
		{
		case ParsingSettings::degrees:
			result *= (180.0 / get_pi_value());
			break;
		case ParsingSettings::gradians:
			result *= (200.0 / get_pi_value());
			break;
		}
		break;
	case ParsingSettings::gradians:
		switch (destUnits)
		{
		case ParsingSettings::degrees:
			result *= (180.0 / 200.0);
			break;
		case ParsingSettings::radians:
			result *= (get_pi_value() / 200.0);
			break;
		}
		break;
	default:
		switch (destUnits)
		{
		case ParsingSettings::radians:
			result *= (get_pi_value() / 180.0);
			break;
		case ParsingSettings::gradians:
			result *= (200.0 / 180.0);
			break;
		}
		break;
	}

	return result;
}

void ParserMathematicalFunctions::deltaVector(vector<calcFloat>& vec)
{
	for (unsigned i = 0; i < (vec.size() - 1); i++)
	{
		vec[i] = vec[i + 1] - vec[i];
	}

	vec.pop_back();
}

calcFloat ParserMathematicalFunctions::sum(const vector<calcFloat>& vec)
{
	calcFloat total = 0.0;

	for (unsigned i = 0; i < vec.size(); i++)
	{
		total += vec[i];
	}

	return total;
}

calcFloat ParserMathematicalFunctions::product(const vector<calcFloat>& vec)
{
	calcFloat total = 1.0;

	for (unsigned i = 0; i < vec.size(); i++)
	{
		total *= vec[i];
	}

	return total;
}

calcFloat ParserMathematicalFunctions::average(const vector<calcFloat>& vec)
{
	calcFloat total = 0.0;

	for (unsigned i = 0; i < vec.size(); i++)
	{
		total += vec[i];
	}

	return total / vec.size();
}

calcFloat ParserMathematicalFunctions::stddev(const vector<calcFloat>& vec)
{
	return sqrt(variance(vec));
}

calcFloat ParserMathematicalFunctions::variance(const vector<calcFloat>& vec)
{
	calcFloat averageVal, averageSquare;
	vector<calcFloat> squaredSet;

	averageVal = average(vec);

	squaredSet.resize(vec.size());

	for (unsigned i = 0; i < vec.size(); i++)
	{
		squaredSet[i] = ((vec[i] - averageVal) * (vec[i] - averageVal));
	}

	averageSquare = average(squaredSet);

	return averageSquare;
}

calcFloat ParserMathematicalFunctions::max(const std::vector<calcFloat>& vec)
{
	if (vec.size() >= 1)
	{
		calcFloat maxVal = vec[0];

		for (unsigned i = 1; i < vec.size(); i++)
		{
			if (vec[i] > maxVal)
			{
				maxVal = vec[i];
			}
		}

		return maxVal;
	}
	else
	{
		throw exception("Max operation not defined for empty vectors");
	}
}

calcFloat ParserMathematicalFunctions::min(const std::vector<calcFloat>& vec)
{
	if (vec.size() >= 1)
	{
		calcFloat minVal = vec[0];

		for (unsigned i = 1; i < vec.size(); i++)
		{
			if (vec[i] < minVal)
			{
				minVal = vec[i];
			}
		}

		return minVal;
	}
	else
	{
		throw exception("Min operation not defined for empty vectors");
	}
}

calcSignedInt ParserMathematicalFunctions::binomialCoefficient(unsigned degree, unsigned index)
{
	// calcSignedInt numeratorValue, denominatorValue;

#ifdef MULTIPRECISION
	/* calcSignedInt convertedDegree(degree);
	mpz_pow_ui(numeratorValue.backend().data(), convertedDegree.backend().data(), index);
	mpz_fac_ui(denominatorValue.backend().data(), index); */

	calcSignedInt resultValue;
	mpz_bin_uiui(resultValue.backend().data(), degree, index);
	return resultValue;
#else
	return (factorial(degree) / (factorial(degree - index) * factorial(index)));
#endif
}

calcFloat ParserMathematicalFunctions::factorial(calcFloat value)
{
	calcFloat result;

	if (value >= 0)
	{
		/* if(value.convert_to<calcSignedInt>() == value)
		{
		for(int i = 2; i <= value; i++)
		{
		result *= i;
		}
		}
		else
		{  */

#ifdef MULTIPRECISION
		result = mpfr::tgamma(value + 1.0);
#else
		result = tgamma(value + 1.0);
#endif
		/* try
		{
		result = boost::math::tgamma(value + 1.0);
		}
		catch(const exception& e)
		{
		result = -1.0;
		} */

		// }

	}
	else
	{
		result = -1.0;
	}

	return result;
}

calcSignedInt ParserMathematicalFunctions::factorial(unsigned value)
{
	calcSignedInt resultValue;

#ifdef MULTIPRECISION
	mpz_fac_ui(resultValue.backend().data(), value);
#else
	resultValue = 1;

	for (calcSignedInt i = 2; i <= value; i++)
	{
		resultValue *= i;
	}
#endif

	return resultValue;
}

bool ParserMathematicalFunctions::findBisectionSolution(const SingleVariableFunction* evaluationFunction,
	calcFloat lBound,
	calcFloat hBound,
	calcFloat& result)
{
	calcFloat lBoundVal, average, averageVal, hBoundVal;
	bool reachedPrecisionLimit = false, noSolution = false, foundSolution = false;

	lBoundVal = evaluationFunction->evaluate(lBound);
	hBoundVal = evaluationFunction->evaluate(hBound);

	while (!reachedPrecisionLimit && !noSolution && !foundSolution)
	{
		average = (lBound + hBound) / 2;

		/* calcFloat hBoundCopy = hBound, lBoundCopy = lBound;
		// differenceVal.setPrecision(differenceVal.getPrecision() - mpfr::digits2bits(1));
		hBoundCopy.setPrecision(hBoundCopy.getPrecision() - 1);
		lBoundCopy.setPrecision(lBoundCopy.getPrecision() - 1); */

		if (hBound == average || lBound == average
#ifdef MULTIPRECISION
			|| (hBound.get_exp() < -(hBound.getPrecision()) && lBound.get_exp() < -(lBound.getPrecision())))
#else
			|| (abs(hBound) < numeric_limits<calcFloat>::min() && abs(lBound) < numeric_limits<calcFloat>::min()))
#endif
		{
			reachedPrecisionLimit = true;
			result = average;
		}
		else
		{
			averageVal = evaluationFunction->evaluate(average);

			// cout << lBound << ' ' << lBoundVal << ';' << average << ' ' << averageVal << ';' << hBound << ' ' << hBoundVal << endl;

			if (lBoundVal == 0.0)
			{
				result = lBound;
				foundSolution = true;
			}
			else if (hBoundVal == 0.0)
			{
				result = hBound;
				foundSolution = true;
			}
			else if (averageVal == 0.0)
			{
				result = average;
				foundSolution = true;
			}
#ifdef MULTIPRECISION
			else if (mpfr::signbit(lBoundVal) != mpfr::signbit(averageVal) && !mpfr::iszero(lBoundVal) && !mpfr::iszero(averageVal))
#else
			else if (signbit(lBoundVal) != signbit(averageVal)) 
#endif
			{
				hBound = average;
				hBoundVal = averageVal;
			}
#ifdef MULTIPRECISION
			else if (mpfr::signbit(averageVal) != mpfr::signbit(hBoundVal) && !mpfr::iszero(averageVal) && !mpfr::iszero(hBoundVal))
#else
			else if (signbit(averageVal) != signbit(hBoundVal))
#endif
			{
				lBound = average;
				lBoundVal = averageVal;
			}
			else
			{
				noSolution = true;
			}
		}
	}

	return !noSolution;
	/*	if (baseCall)
	{
	sort(solutions.begin(), solutions.end());
	} */
}

void ParserMathematicalFunctions::findMultipleBisectionSolutions(const SingleVariableFunction * evaluationFunction, calcFloat lBound, calcFloat hBound, std::vector<calcFloat>& solutions, unsigned numIntervals)
{
	calcFloat currentResult,
		delta = ((hBound - lBound) / numIntervals),
		currentLowerBound = lBound;

	for (unsigned i = 0; i < numIntervals; i++, currentLowerBound += delta)
	{
		bool solutionFound = findBisectionSolution(evaluationFunction, currentLowerBound, currentLowerBound + delta, currentResult);

		if (solutionFound && !(solutions.size() >= 1 && currentResult == solutions.back()))
		{
			solutions.push_back(currentResult);
		}
	}
}

void ParserMathematicalFunctions::primeFactorization(vector<calcFloat>& results, calcSignedInt number)
{
	bool factorFound;
	calcSignedInt lastFactor = 3;

	while (number % 2 == 0)
	{
		results.push_back(2);
		number /= 2;
	}

	if (number > 1)
	{
		do
		{
			factorFound = false;

			calcFloat numberFloat;
			intToFloat(number, numberFloat);

			calcFloat squareRootFloat = floor(sqrt(numberFloat));
			calcSignedInt squareRoot;
			floatToInt(squareRootFloat, squareRoot);

			for (calcSignedInt factor = lastFactor; !factorFound && factor <= squareRoot; factor += 2)
			{
				if (number % factor == 0)
				{
					calcFloat factorFloat;
					intToFloat(factor, factorFloat);

					results.push_back(factorFloat);

					number /= factor;

					// cout << "Number: " << number << endl;

					lastFactor = factor;
					factorFound = true;
				}
			}
		} while (factorFound);

		calcFloat remainingNumberFloat;
		intToFloat(number, remainingNumberFloat);

		results.push_back(remainingNumberFloat);
	}
}

bool ParserMathematicalFunctions::isPrime(calcSignedInt number)
{
	calcFloat numberFloat;
	intToFloat(number, numberFloat);

	calcFloat squareRootFloat = floor(sqrt(numberFloat));
	calcSignedInt squareRoot;
	floatToInt(squareRootFloat, squareRoot);

	bool primeNum = true;

	if (number % 2 == 0 && number != 2)
	{
		primeNum = false;
	}
	else
	{
		for (calcSignedInt factor = 3; primeNum && factor <= squareRoot; factor += 2)
		{
			if (number % factor == 0)
			{
				primeNum = false;
			}
		}
	}
	
	return primeNum;
}

calcSignedInt ParserMathematicalFunctions::leastCommonMultiple(calcSignedInt firstNum, calcSignedInt secondNum)
{
	return (firstNum / greatestCommonDenominator(firstNum, secondNum)) * secondNum;
}

calcSignedInt ParserMathematicalFunctions::greatestCommonDenominator(calcSignedInt firstNum, calcSignedInt secondNum)
{
	while (firstNum != secondNum)
	{
		if (firstNum < secondNum)
		{
			secondNum -= ((secondNum - firstNum) / firstNum + (((secondNum - firstNum) % firstNum > 0) ? 1 : 0)) * firstNum;
		}
		else
		{
			firstNum -= ((firstNum - secondNum) / secondNum + (((firstNum - secondNum) % secondNum > 0) ? 1 : 0)) * secondNum;
		}

		cout << firstNum << ' ' << secondNum << endl;
	}

	return firstNum;
}

calcFloat ParserMathematicalFunctions::scaleNum(calcFloat oldMin, calcFloat oldMax, calcFloat newMin, calcFloat newMax, calcFloat value)
{
	calcFloat slope = (newMax - newMin) / (oldMax - oldMin);
	calcFloat xIntercept = newMin - (oldMin * slope);

	return (value * slope) + xIntercept;
}