#pragma once
#ifndef BUILT_IN_FUNCTIONS_H
#define BUILT_IN_FUNCTIONS_H


#include <vector>
#include <variant>
#include <map>
#include <string>

#include "CalcObj.h"
#include "FunctionSignature.h"
#include "ParsingSettings.h"
#include "BuiltInConstants.h"
#include "ParserCore.h"
#include "RandomNumberGeneration.h"
#include "BuiltInFunctionTypes.h"

template<typename FnType>
void DEPRECATED_FUNCTION_WARNING(const std::string& functionName, const std::string& replacementName = std::string())
{
	static bool warningIssued = false;

	if(!warningIssued)
	{
		std::cerr << "WARNING: The \"" << functionName << "\" function is deprecated and will likely be removed in a future release." << std::endl;

		if(!replacementName.empty())
		{
			std::cerr << "         Please consider using " << replacementName << " instead." << std::endl;
		}
		
		warningIssued = true;
	}
}

class SineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Sine type error");

		return sin(ParserMathematicalFunctions::convertAngle(parser->parsingSettings.parseAngleMode, ParsingSettings::radians, floatArg));
	}
};

class CosineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Cosine type error");

		return cos(ParserMathematicalFunctions::convertAngle(parser->parsingSettings.parseAngleMode, ParsingSettings::radians, floatArg));
	}
};

class TangentFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Tangent type error");

		return tan(ParserMathematicalFunctions::convertAngle(parser->parsingSettings.parseAngleMode, ParsingSettings::radians, floatArg));
	}
};

class ArcsineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Arcsine type error");

		return ParserMathematicalFunctions::convertAngle(ParsingSettings::radians, parser->parsingSettings.parseAngleMode, asin(floatArg));
	}
};

class ArccosineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Arccosine type error");

		return ParserMathematicalFunctions::convertAngle(ParsingSettings::radians, parser->parsingSettings.parseAngleMode, acos(floatArg));
	}
};

class ArctangentFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Arctangent type error");

		return ParserMathematicalFunctions::convertAngle(ParsingSettings::radians, parser->parsingSettings.parseAngleMode, atan(floatArg));
	}
};

class HypSineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Hyperbolic sine type error");

		return sinh(floatArg);
	}
};

class HypCosineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Hyperbolic cosine type error");

		return cosh(floatArg);
	}
};

class HypTangentFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Hyperbolic tangent type error");

		return tanh(floatArg);
	}
};

class HypArcsineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Hyperbolic arcsine type error");

		return log(floatArg + sqrt(1 + (floatArg * floatArg)));
	}
};

class HypArccosineFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Hyperbolic arcsine type error");

		if(floatArg >= 1)
		{
			return log(floatArg + (sqrt(floatArg + 1) * sqrt(floatArg - 1)));
		}
		else
		{
			throw BuiltInFunctionDomainError("argument must be at least 1");
		}
	}
};

class HypArctangentFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Hyperbolic arctangent type error");

		if (floatArg > -1 && floatArg < 1)
		{
			return (log(1 + floatArg) - log(1 - floatArg)) / 2;;
		}
		else
		{
			throw BuiltInFunctionDomainError("argument must be in (-1, 1)");
		}
	}
};

class SqrtFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Square root type error");

		if(floatArg >= 0)
		{
			return sqrt(floatArg);
		}
		else
		{
			throw BuiltInFunctionDomainError("argument must be non-negative");
		}
	}
};

class LnFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Natural log type error");

		if (floatArg > 0)
		{
			return log(floatArg);
		}
		else
		{
			throw BuiltInFunctionDomainError("argument must be positive");
		}
	}
};

class LogFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Common log type error");

		if (floatArg > 0)
		{
			return log10(floatArg);
		}
		else
		{
			throw BuiltInFunctionDomainError("argument must be positive");
		}
	}
};

class LogBaseFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat valueArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Logbase value argument type error"),
			baseArg = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Logbase base argument type error");

		if (valueArg <= 0)
		{
			throw BuiltInFunctionDomainError("value argument must be positive");
		}
		else if(baseArg <= 0 || baseArg == 1.0)
		{
			throw BuiltInFunctionDomainError("base argument must be positive and cannot be 1");
		}
		else
		{
			return (log10(valueArg) / log10(baseArg));
		}
	}
};

class AnsFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Ans type error");
		unsigned floatArgInt;

		try
		{
			floatArgInt = floatToUnsigned(floatArg);
		}
		catch (const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("argument must be a non-negative integer");
		}

		if(floatArgInt >= 1 && parser->resultHistory.size() >= floatArgInt)
		{
			CalcObj historyValue = parser->resultHistory[parser->resultHistory.size() - floatArgInt];

#ifdef MULTIPRECISION
			historyValue.set_precision(mpfr::mpreal::get_default_prec());
#endif
			return historyValue;
		}
		else
		{
			throw BuiltInFunctionDomainError("argument is out of history range");
		}
	}
};

class DerivFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ false, false, true, true, true },
			{ 3, 4, 5 },
			false
		};
	}

	calcFloat calcDerivative(
		const ExpressionParser* parser,
		std::string expression,
		std::string variable,
		calcFloat value,
		calcFloat delta,
		const ParsingContext_Export& parsingContext,
		std::string parentExpression,
		unsigned derivNum) const
	{
		ParsingContext_Export newContext = ParsingContext_Export(&parsingContext);

		string typeErrorMsg = "Derivative type error";

		vector<calcFloat> diffValues;
		calcFloat startVal = value - (derivNum * delta / 2), currentVal = startVal, currentFnVal;

		for (unsigned i = 0; i < (derivNum + 1); i++)
		{
			newContext.setVariable(variable, currentVal);

			currentFnVal = parser->parseArithmetic(expression, newContext, parentExpression).getVerifiedFloat(typeErrorMsg);

			diffValues.push_back(currentFnVal);

			currentVal += delta;
		}

		while (diffValues.size() > 1)
		{
			ParserMathematicalFunctions::deltaVector(diffValues);

			for (unsigned i = 0; i < diffValues.size(); i++)
			{
				diffValues[i] /= delta;
			}
		}

		return diffValues[0];
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat value = std::get<CalcObj>(arguments[2]).getVerifiedFloat("Derivative value type error"),
			delta = 1.0e-4;
		unsigned derivNum = 1;

		if(arguments.size() >= 4)
		{
			try
			{
				derivNum = floatToUnsigned(std::get<CalcObj>(arguments[3]).getVerifiedFloat("Derivative number type error"));
			}
			catch (const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("derivative number must be a non-negative integer");
			}

			if(arguments.size() >= 5)
			{
				delta = std::get<CalcObj>(arguments[4]).getVerifiedFloat("Derivative delta type error");

				if (delta <= 0)
				{
					throw BuiltInFunctionDomainError("delta argument must be positive");
				}
			}
		}

		return calcDerivative(parser, std::get<std::string>(arguments[0]), std::get<std::string>(arguments[1]),
			value, delta, parsingContext, fnExpression, derivNum);
	}
};

class IntegralFunction : public BuiltInFunctionDefinition
{
#ifdef MULTIPRECISION
	vector<calcFloat> gaussNodes, gaussWeights;
	unsigned cachePrecision = 0;
#endif
	unsigned cachedEstimationPoints = 0;

	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ false, false, true, true },
			{ 4 },
			false
		};
	}

	class LegendrePolynomial : public ParserMathematicalFunctions::SingleVariableFunction
	{
	private:
		unsigned polynomialOrder;
		vector<calcFloat> summationFactors;
		long calculationPrecision;
	public:
		LegendrePolynomial(unsigned polynomialOrder) :
			polynomialOrder(polynomialOrder)
		{
			this->summationFactors.resize(this->polynomialOrder + 1);
			long maxSummationFactorPrecision = 0;

			for (unsigned i = 0; i <= this->polynomialOrder; i++)
			{
				calcSignedInt integerResult = (ParserMathematicalFunctions::binomialCoefficient(this->polynomialOrder, i)
					* ParserMathematicalFunctions::binomialCoefficient(this->polynomialOrder + i, i));
				intToFloat(integerResult, this->summationFactors[i]);

#ifdef MULTIPRECISION
				if (this->summationFactors[i].get_exp() > maxSummationFactorPrecision)
				{
					maxSummationFactorPrecision = this->summationFactors[i].get_exp();
				}
#endif
			}

#ifdef MULTIPRECISION
			this->calculationPrecision = mpfr::mpreal::get_default_prec() + maxSummationFactorPrecision; // + mpfr::digits2bits((long)(log10(this->polynomialOrder) + 1));
#endif
		}

		calcFloat evaluate(calcFloat value) const
		{
			// mpfr::mpreal::set_default_prec(originalPrecision + mpfr::digits2bits(20));

			calcFloat sumValue, currentPower = 1.0, iterationFactor;

#ifdef MULTIPRECISION
			sumValue.setPrecision(this->calculationPrecision);
			currentPower.setPrecision(this->calculationPrecision);
			iterationFactor.setPrecision(this->calculationPrecision);
#endif

			sumValue = 0.0;
			// currentPower = 1.0;
			iterationFactor = (value - 1.0) / 2.0;

			for (unsigned i = 0; i <= this->polynomialOrder; i++)
			{
#ifndef MULTIPRECISION
				currentPower = pow(iterationFactor, i);
#endif
				sumValue += (this->summationFactors[i] * currentPower);

#ifdef MULTIPRECISION
				currentPower *= iterationFactor;
#endif
			}

#ifdef MULTIPRECISION
			sumValue.setPrecision(mpfr::mpreal::get_default_prec());
#endif

			return sumValue;
		}
	};

	calcFloat gaussIntegral(const ExpressionParser* parser,
		std::string expression,
		std::string variable,
		calcFloat lower,
		calcFloat upper,
		unsigned estimationPoints,
		const ParsingContext_Export& parsingContext,
		std::string parentExpression)
	{
		ParsingContext_Export newContext = ParsingContext_Export(&parsingContext);
		calcFloat total = 0.0, ptVal;

		string typeErrorMsg = "Integral type error";

#ifdef MULTIPRECISION
		if (estimationPoints != this->cachedEstimationPoints || mpfr::mpreal::get_default_prec() != cachePrecision)
		{
			gaussNodes.clear();
			gaussWeights.clear();

			LegendrePolynomial nodeFindingPolynomial(estimationPoints), weightDenominatorPolynomial(estimationPoints - 1);

			ParserMathematicalFunctions::findMultipleBisectionSolutions(&nodeFindingPolynomial, 0.0, 1.0, gaussNodes, estimationPoints * estimationPoints);

			unsigned originalNodeResultsSize = gaussNodes.size();

			for (unsigned i = 0; i < originalNodeResultsSize; i++)
			{
				if (i >= 1 || (!mpfr::iszero(gaussNodes[i]) && gaussNodes[i].get_exp() >= -(gaussNodes[i].getPrecision())))
				{
					gaussNodes.push_back(-gaussNodes[i]);
				}
			}

			gaussWeights.resize(gaussNodes.size());

			for (unsigned i = 0; i < gaussNodes.size(); i++)
			{
				gaussWeights[i] = mpfr::mul_2ui(1.0 - mpfr::sqr(gaussNodes[i]), 1)
					/ mpfr::sqr(estimationPoints * weightDenominatorPolynomial.evaluate(gaussNodes[i]));
			}

			cachedEstimationPoints = estimationPoints;
			cachePrecision = mpfr::mpreal::get_default_prec();
		}
#endif


		if (lower != upper)
		{
			for (size_t i = 0; i < gaussNodes.size(); i++)
			{
				newContext.setVariable(variable, ParserMathematicalFunctions::scaleNum(-1.0, 1.0, lower, upper, gaussNodes[i]));
				ptVal = parser->parseArithmetic(expression, newContext, parentExpression).getVerifiedFloat(typeErrorMsg);

				total += (ptVal * gaussWeights[i]);
			}
		}

		total *= (upper - lower) / 2.0;

		return total;
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		string typeErrorMsg = "Integral type error";

		calcFloat lower = std::get<CalcObj>(arguments[2]).getVerifiedFloat("Integral lower bound type error"),
			upper = std::get<CalcObj>(arguments[3]).getVerifiedFloat("Integral upper bound type error");

#ifdef MULTIPRECISION
		return gaussIntegral(parser, std::get<std::string>(arguments[0]), std::get<std::string>(arguments[1]), lower, upper, 50, parsingContext, fnExpression);
#else
		return gaussIntegral(parser, std::get<std::string>(arguments[0]), std::get<std::string>(arguments[1]), lower, upper, 10, parsingContext, fnExpression);
#endif
	}
};

class SolveFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ false, false, true, true, true },
			{ 4, 5 },
			false
		};
	}

	class SingleVariableParsedFunction : public ParserMathematicalFunctions::SingleVariableFunction
	{
	private:
		std::string expression,
			parentExpression,
			variable,
			typeErrorStr;

		const ExpressionParser* parser;
		const ParsingContext_Export* parsingContext;

	public:
		SingleVariableParsedFunction(std::string expression, std::string parentExpression, std::string variable, std::string typeErrorStr,
			const ExpressionParser* parser, const ParsingContext_Export* parsingContext):
			expression(expression),
			parentExpression(parentExpression),
			variable(variable),
			typeErrorStr(typeErrorStr),
			parser(parser),
			parsingContext(parsingContext)
		{
		}

		calcFloat evaluate(calcFloat value) const
		{
			ParsingContext_Export newContext = ParsingContext_Export(this->parsingContext);
			newContext.setVariable(this->variable, value);

			return (this->parser->parseArithmetic(this->expression, newContext, this->parentExpression).getVerifiedFloat(this->typeErrorStr));
		}
	};

	void bisectionSolve(const ExpressionParser* parser,
		std::string expression,
		std::string variable,
		calcFloat lBound,
		calcFloat hBound,
		std::vector<calcFloat>& solutions,
		unsigned numIntervals,
		const ParsingContext_Export& parsingContext,
		std::string parentExpression) const
	{
		string typeErrorStr = "Equation solver type error";

		SingleVariableParsedFunction functionEvaluator = SingleVariableParsedFunction(expression, parentExpression, variable, typeErrorStr, parser, &parsingContext);

		ParserMathematicalFunctions::findMultipleBisectionSolutions(&functionEvaluator, lBound, hBound, solutions, numIntervals);
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		unsigned intervals = 16;

		calcFloat lBound = std::get<CalcObj>(arguments[2]).getVerifiedFloat("Solve function lower bound type error"),
			hBound = std::get<CalcObj>(arguments[3]).getVerifiedFloat("Solve function upper bound type error");

		if(hBound <= lBound)
		{
			throw BuiltInFunctionDomainError("solver upper bound must be greater than solver lower bound");
		}

		if(arguments.size() >= 5)
		{
			try
			{
				intervals = floatToUnsigned(std::get<CalcObj>(arguments[4]).getVerifiedFloat("Solver interval number type error"));
			}
			catch(const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("solver interval number is not a valid integer");
			}

			if(intervals == 0)
			{
				throw BuiltInFunctionDomainError("solver interval number cannot be zero");
			}
		}

		std::vector<calcFloat> results;
		CalcObj resultObj;

		this->bisectionSolve(parser, std::get<string>(arguments[0]), std::get<string>(arguments[1]), lBound, hBound, results, intervals, parsingContext, fnExpression);
		resultObj.set_list(results);
		return resultObj;
	};
};

class AbsFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 }, // TODO: List norm?
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Absolute value type error");

		return fabs(floatArg);
	}
};

class CeilFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 1, 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat value = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Ceiling value type error");

		if(arguments.size() == 1)
		{
			return ceil(value);
		}
		else
		{
			calcFloat ceilingMultiple = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Ceiling multiple type error");

			if(ceilingMultiple == 0.0)
			{
				throw BuiltInFunctionDomainError("Ceiling multiple cannot be zero");
			}
			else
			{
				return ceil(value / ceilingMultiple) * ceilingMultiple;
			}
		}
	}
};

class FloorFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 1, 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat value = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Floor value type error");

		if (arguments.size() == 1)
		{
			return floor(value);
		}
		else
		{
			calcFloat floorMultiple = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Floor multiple type error");

			if (floorMultiple == 0.0)
			{
				throw BuiltInFunctionDomainError("Floor multiple cannot be zero");
			}
			else
			{
				return floor(value / floorMultiple) * floorMultiple;
			}
		}
	}
};

class RoundFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 1, 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const
	                 std::string& fnExpression) override
	{
		calcFloat value = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Round value type error");

		if (arguments.size() == 1)
		{
			return round(value);
		}
		else
		{
			calcFloat roundMultiple = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Round multiple type error");

			if (roundMultiple == 0.0)
			{
				throw BuiltInFunctionDomainError("Round multiple cannot be zero");
			}
			else
			{
				return round(value / roundMultiple) * roundMultiple;
			}
		}
	}
};

class PrimeFctFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Prime factorization function type error");
		calcSignedInt intArg;

		try
		{
			floatToInt(floatArg, intArg);
		}
		catch(const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("argument is not a valid integer");
		}

		if(intArg <= 1)
		{
			throw BuiltInFunctionDomainError("argument must be 2 or larger");
		}
		else
		{
			std::vector<calcFloat> results;
			CalcObj resultObj;

			ParserMathematicalFunctions::primeFactorization(results, intArg);
			resultObj.set_list(results);
			return resultObj;
		}
	}
};

class IsPrimeFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat floatArg = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Prime checking function type error");
		calcSignedInt intArg;

		try
		{
			floatToInt(floatArg, intArg);
		}
		catch (const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("argument is not a valid integer");
		}

		if (intArg <= 1)
		{
			throw BuiltInFunctionDomainError("argument must be 2 or larger");
		}
		else
		{
			return boolToFloat(ParserMathematicalFunctions::isPrime(intArg));
		}
	}
};

class GCDFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 }, // TODO: support arbitrarily many numbers
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat firstVal = std::get<CalcObj>(arguments[0]).getVerifiedFloat("GCD first argument type error"),
			secondVal = std::get<CalcObj>(arguments[1]).getVerifiedFloat("GCD second argument type error");

		calcSignedInt firstValInt, secondValInt, resultInt;

		if(firstVal <= 0.0)
		{
			throw BuiltInFunctionDomainError("first argument must be positive");
		}
		else if(secondVal <= 0.0)
		{
			throw BuiltInFunctionDomainError("second argument must be positive");
		}
		else
		{
			try
			{
				floatToInt(firstVal, firstValInt);
			}
			catch(const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("first argument is not a valid integer");
			}

			try
			{
				floatToInt(secondVal, secondValInt);
			}
			catch (const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("first argument is not a valid integer");
			}
		}

#ifdef MULTIPRECISION
		resultInt = gcd(firstValInt, secondValInt);
#else
		resultInt = ParserMathematicalFunctions::greatestCommonDenominator(firstValInt, secondValInt);
#endif

		calcFloat buffer;
		intToFloat(resultInt, buffer);

		return buffer;
	}
};

class LCMFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 }, // TODO: support arbitrarily many numbers
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat firstVal = std::get<CalcObj>(arguments[0]).getVerifiedFloat("LCM first argument type error"),
			secondVal = std::get<CalcObj>(arguments[1]).getVerifiedFloat("LCM second argument type error");

		calcSignedInt firstValInt, secondValInt, resultInt;

		if (firstVal <= 0.0)
		{
			throw BuiltInFunctionDomainError("first argument must be positive");
		}
		else if (secondVal <= 0.0)
		{
			throw BuiltInFunctionDomainError("second argument must be positive");
		}
		else
		{
			try
			{
				floatToInt(firstVal, firstValInt);
			}
			catch (const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("first argument is not a valid integer");
			}

			try
			{
				floatToInt(secondVal, secondValInt);
			}
			catch (const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("second argument is not a valid integer");
			}
		}

#ifdef MULTIPRECISION
		resultInt = lcm(firstValInt, secondValInt);
#else
		resultInt = ParserMathematicalFunctions::leastCommonMultiple(firstValInt, secondValInt);
#endif

		calcFloat buffer;
		intToFloat(resultInt, buffer);

		return buffer;
	}
};

class RandFunction : public BuiltInFunctionDefinition
{
	shared_ptr<RandomNumberGenerator> RNGPtr;
public:

	ParameterSpecs getParameterSpecs() override
	{
		return {
			{},
			{ 0 },
			false
		};
	}

	RandFunction(shared_ptr<RandomNumberGenerator> RNGPtr):
		RNGPtr(RNGPtr)
	{}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat result;
		RNGPtr->generateUniformFloat(result);
		return result;
	}
};

class RandIntFunction : public BuiltInFunctionDefinition
{
	shared_ptr<RandomNumberGenerator> RNGPtr;
public:

	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	RandIntFunction(shared_ptr<RandomNumberGenerator> RNGPtr):
		RNGPtr(RNGPtr)
	{}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat minVal = std::get<CalcObj>(arguments[0]).getVerifiedFloat("Random integer first argument type error"),
			maxVal = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Random integer second argument type error");

		calcSignedInt minValInt, maxValInt, resultInt;

		if(minVal > maxVal)
		{
			throw BuiltInFunctionDomainError("minimum argument cannot be greater than maximum argument");
		}

		try
		{
			floatToInt(minVal, minValInt);
		}
		catch (const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("first (minimum) argument is not a valid integer");
		}

		try
		{
			floatToInt(maxVal, maxValInt);
		}
		catch (const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("second (maximum) argument is not a valid integer");
		}

		RNGPtr->generateUniformInteger(minValInt, maxValInt, resultInt);
		calcFloat floatResult;
		intToFloat(resultInt, floatResult);

		return floatResult;
	}
};

class SeqFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ false, false, true, true, true },
			{ 4, 5 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat lower = std::get<CalcObj>(arguments[2]).getVerifiedFloat("Sequence lower bound type error"),
			upper = std::get<CalcObj>(arguments[3]).getVerifiedFloat("Sequence upper bound type error"),
			delta = 1.0;

		if(arguments.size() >= 5)
		{
			delta = std::get<CalcObj>(arguments[4]).getVerifiedFloat("Sequence delta type error");

			if(delta == 0.0)
			{
				throw BuiltInFunctionDomainError("delta cannot be zero");
			}
		}

		if (!((upper > lower && delta > 0.0) || (upper < lower && delta < 0.0) || (upper == lower)))
		{
			throw BuiltInFunctionDomainError("invalid delta sign");
		}

		unsigned resultSize;
		std::vector<CalcObj> result;
		ParsingContext_Export newContext = ParsingContext_Export(&parsingContext);
		CalcObj thisPt;

		try
		{
			resultSize = floatToUnsigned(floor(abs((upper - lower) / delta))) + 1;
		}
		catch (const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("invalid range");
		}

		result.resize(resultSize);

		for (unsigned thisIndex = 0; thisIndex < resultSize; thisIndex++)
		{
			calcFloat thisVal = lower + (thisIndex * delta);

			newContext.setVariable(std::get<string>(arguments[1]), thisVal);
			result[thisIndex] = parser->parseArithmetic(std::get<string>(arguments[0]), newContext, fnExpression);
		}

		std::vector<calcFloat> floatResult;
		CalcObj returnObj;

		toVerifiedFloatVector(result, floatResult, "Sequence evaluation type error");
		returnObj.set_list(floatResult);
		return returnObj;
	}
};

class ForeachFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, false },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		CalcObj listObj = std::get<CalcObj>(arguments[0]);

		vector<calcFloat> listValues, newValues;

		ParsingContext_Export functionContext = ParsingContext_Export(&parsingContext);

		if (listObj.get_type() == TYPE_LIST)
		{
			listObj.get_list(listValues);

			newValues.resize(listValues.size());

			functionContext.setVariable("length", CalcObj(listValues.size()));

			for (unsigned i = 0; i < listValues.size(); i++)
			{
				functionContext.setVariable("index", CalcObj(i));
				functionContext.setVariable("value", listValues[i]);

				newValues[i] = parser->parseArithmetic(std::get<string>(arguments[1]), functionContext, fnExpression).getVerifiedFloat("Foreach evaluation type error");
			}

			CalcObj newObj;
			newObj.set_list(newValues);
			return newObj;
		}
		else
		{
			throw type_error("Foreach input list type error");
		}
	}
};

class GetIndexFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		CalcObj listObj = std::get<CalcObj>(arguments[0]);

		if (listObj.get_type() == TYPE_LIST)
		{
			calcFloat floatIndex = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Getindex index type error");
			unsigned index;

			try
			{
				index = floatToUnsigned(floatIndex);
			}
			catch(const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("index must be a non-negative integer");
			}

			try
			{
				return listObj.get_list_index(index);
			}
			catch (const std::domain_error& ex)
			{
				throw BuiltInFunctionDomainError(ex.what());
			}
		}
		else
		{
			throw type_error("Getindex input list type error");
		}
	}
};

class SetIndexFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true, true },
			{ 3 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		CalcObj listObj = std::get<CalcObj>(arguments[0]);

		if (listObj.get_type() == TYPE_LIST)
		{
			calcFloat floatIndex = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Setindex index type error"),
						floatValue = std::get<CalcObj>(arguments[2]).getVerifiedFloat("Setindex value type error");
			unsigned index;

			try
			{
				index = floatToUnsigned(floatIndex);
			}
			catch (const NumericConversionError&)
			{
				throw BuiltInFunctionDomainError("index must be a non-negative integer");
			}

			try
			{
				listObj.set_list_index(index, floatValue);
				return listObj;
			}
			catch (const std::domain_error& ex)
			{
				throw BuiltInFunctionDomainError(ex.what());
			}
		}
		else
		{
			throw type_error("Setindex input list type error");
		}
	}
};

class LengthFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		CalcObj listObj = std::get<CalcObj>(arguments[0]);

		if (listObj.get_type() == TYPE_LIST)
		{
			return calcFloat(listObj.get_list_length());
		}
		else
		{
			throw type_error("Length type error");
		}
	}
};

class ConcatFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			true,
			true
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		CalcObj concatResult = std::get<CalcObj>(arguments[0]);

		for(unsigned i = 1; i < arguments.size(); i++)
		{
			concatResult.concat_list(std::get<CalcObj>(arguments[i]));
		}

		return concatResult;
	}
};

class SublistFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true, true },
			{ 3 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		CalcObj origList = std::get<CalcObj>(arguments[0]);

		calcFloat startIndex = std::get<CalcObj>(arguments[1]).getVerifiedFloat("Sublist start index type error"),
			endIndex = std::get<CalcObj>(arguments[2]).getVerifiedFloat("Sublist end index type error");

		unsigned startIndexInt,
			endIndexInt;

		try
		{
			startIndexInt = floatToUnsigned(startIndex);
		}
		catch(const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("start index must be an unsigned integer");
		}

		try
		{
			endIndexInt = floatToUnsigned(endIndex);
		}
		catch (const NumericConversionError&)
		{
			throw BuiltInFunctionDomainError("end index must be an unsigned integer");
		}

		try
		{
			origList.sublist(startIndexInt, endIndexInt);
			return origList;
		}
		catch(const std::domain_error& ex)
		{
			throw BuiltInFunctionDomainError(ex.what());
		}
	}
};

class FilterFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, false },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		CalcObj listObj = std::get<CalcObj>(arguments[0]);

		vector<calcFloat> listValues, newValues;

		ParsingContext_Export functionContext = ParsingContext_Export(&parsingContext);

		if (listObj.get_type() == TYPE_LIST)
		{
			listObj.get_list(listValues);

			functionContext.setVariable("length", CalcObj(listValues.size()));

			for (unsigned i = 0; i < listValues.size(); i++)
			{
				functionContext.setVariable("index", CalcObj(i));
				functionContext.setVariable("value", listValues[i]);

				// TODO: Direct comparison of CalcObj w/ numeric types
				calcFloat keepVal = parser->parseArithmetic(std::get<string>(arguments[1]), functionContext, fnExpression).getVerifiedFloat("Filter evaluation type error");

				if(keepVal != 0.0)
				{
					newValues.push_back(listValues[i]);
				}
			}

			CalcObj newObj;
			newObj.set_list(newValues);
			return newObj;
		}
		else
		{
			throw type_error("Filter input list type error");
		}
	}
};

class ListFloatStatFunction : public BuiltInFunctionDefinition
{
	const std::string argTypeErrorMsg;
	calcFloat (*statFunction)(const std::vector<calcFloat>&);

public:
	ListFloatStatFunction(const std::string& argTypeErrorMsg, calcFloat(*statFunction)(const std::vector<calcFloat>&)):
		argTypeErrorMsg(argTypeErrorMsg),
		statFunction(statFunction)
	{
		
	}
	
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			true,
			true
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		vector<calcFloat> parsedParams;

		CalcObj firstParam = std::get<CalcObj>(arguments[0]);

		if (firstParam.get_type() == TYPE_LIST)
		{
			vector<calcFloat> avgList;
			firstParam.get_list(avgList);

			if (!avgList.empty())
			{
				return this->statFunction(avgList);
			}
			else
			{
				throw BuiltInFunctionDomainError("list cannot be empty");
			}
		}
		else
		{
			parsedParams.push_back(firstParam.getVerifiedFloat(this->argTypeErrorMsg));

			for (unsigned i = 1; i < arguments.size(); i++)
			{
				parsedParams.push_back(std::get<CalcObj>(arguments[i]).getVerifiedFloat(this->argTypeErrorMsg));
			}

			return this->statFunction(parsedParams);
		}
	}
};

class AvgFunction : public ListFloatStatFunction
{
public:
	AvgFunction() : ListFloatStatFunction("Average argument type error", ParserMathematicalFunctions::average)
	{}
};

class SumFunction : public ListFloatStatFunction
{
public:
	SumFunction() : ListFloatStatFunction("Sum argument type error", ParserMathematicalFunctions::sum)
	{}
};

class ProductFunction : public ListFloatStatFunction
{
public:
	ProductFunction() : ListFloatStatFunction("Product argument type error", ParserMathematicalFunctions::product)
	{}
};

class StandardDevFunction : public ListFloatStatFunction
{
public:
	StandardDevFunction() : ListFloatStatFunction("Standard deviation argument type error", ParserMathematicalFunctions::stddev)
	{}
};

class VarianceFunction : public ListFloatStatFunction
{
public:
	VarianceFunction() : ListFloatStatFunction("Variance argument type error", ParserMathematicalFunctions::variance)
	{}
};

class MaxFunction : public ListFloatStatFunction
{
public:
	MaxFunction() : ListFloatStatFunction("Max argument type error", ParserMathematicalFunctions::max)
	{}
};

class MinFunction : public ListFloatStatFunction
{
public:
	MinFunction() : ListFloatStatFunction("Min argument type error", ParserMathematicalFunctions::min)
	{}
};

class OrFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ false, false },
			{ 2 },
			true,
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		bool orResult = false;

		for (unsigned i = 0; (i < arguments.size()) && !orResult; i++)
		{
			// TODO: Use truthfulness value for other CalcObj types
			CalcObj parsingResult = parser->parseArithmetic(std::get<string>(arguments[i]), parsingContext, fnExpression);
			orResult = (parsingResult.getVerifiedFloat("Or argument type error") != 0);
		}

		return boolToFloat(orResult);
	}
};

class AndFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ false, false },
			{ 2 },
			true,
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		bool andResult = true;

		for (unsigned i = 0; (i < arguments.size()) && andResult; i++)
		{
			CalcObj parsingResult = parser->parseArithmetic(std::get<string>(arguments[i]), parsingContext, fnExpression);
			andResult = (parsingResult.getVerifiedFloat("And argument type error") != 0);
		}

		return boolToFloat(andResult);
	}
};

class NotFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true },
			{ 1 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                 std::string& fnName, const std::string& fnExpression) override
	{
		bool notResult = (std::get<CalcObj>(arguments[0]).getVerifiedFloat("Not argument type error") == 0);
		return boolToFloat(notResult);
	}
};

class LessThanEqualFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
		std::string& fnName, const std::string& fnExpression) override
	{
		DEPRECATED_FUNCTION_WARNING<decltype(*this)>(fnName, "the less-than-or-equal (<=) operator");
		
		bool compareResult = (std::get<CalcObj>(arguments[0]) <= std::get<CalcObj>(arguments[1]));
		return boolToFloat(compareResult);
	}
};

class LessThanFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
		std::string& fnName, const std::string& fnExpression) override
	{
		DEPRECATED_FUNCTION_WARNING<decltype(*this)>(fnName, "the less-than (<) operator");

		bool compareResult = (std::get<CalcObj>(arguments[0]) < std::get<CalcObj>(arguments[1]));
		return boolToFloat(compareResult);
	}
};

class GreaterThanEqualFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
		std::string& fnName, const std::string& fnExpression) override
	{
		DEPRECATED_FUNCTION_WARNING<decltype(*this)>(fnName, "the greater-than-or-equal (>=) operator");

		bool compareResult = (std::get<CalcObj>(arguments[0]) >= std::get<CalcObj>(arguments[1]));
		return boolToFloat(compareResult);
	}
};

class GreaterThanFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
		std::string& fnName, const std::string& fnExpression) override
	{
		DEPRECATED_FUNCTION_WARNING<decltype(*this)>(fnName, "the greater-than (>) operator");

		bool compareResult = (std::get<CalcObj>(arguments[0]) > std::get<CalcObj>(arguments[1]));
		return boolToFloat(compareResult);
	}
};

class EqualityFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
		std::string& fnName, const std::string& fnExpression) override
	{
		DEPRECATED_FUNCTION_WARNING<decltype(*this)>(fnName, "the equality (=) operator");

		bool compareResult = (std::get<CalcObj>(arguments[0]) == std::get<CalcObj>(arguments[1]));
		return boolToFloat(compareResult);
	}
};

class InequalityFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, true },
			{ 2 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
		std::string& fnName, const std::string& fnExpression) override
	{
		DEPRECATED_FUNCTION_WARNING<decltype(*this)>(fnName, "the inequality (!=) operator");

		bool compareResult = (std::get<CalcObj>(arguments[0]) != std::get<CalcObj>(arguments[1]));
		return boolToFloat(compareResult);
	}
};

class IfFunction : public BuiltInFunctionDefinition
{
	ParameterSpecs getParameterSpecs() override
	{
		return {
			{ true, false, false }, // The last two arguments are lazily evaluated.
			{ 3 },
			false
		};
	}

	CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
		std::string& fnName, const std::string& fnExpression) override
	{
		calcFloat conditionVal = std::get<CalcObj>(arguments[0]).getVerifiedFloat("If condition type error");

		if(conditionVal != 0)
		{
			return parser->parseArithmetic(std::get<string>(arguments[1]), parsingContext, fnExpression);
		}
		else
		{
			return parser->parseArithmetic(std::get<string>(arguments[2]), parsingContext, fnExpression);
		}
	}
};

inline std::map<std::string, BuiltInFunctionDefinition*> getAllBuiltinFunctions()
{
	std::shared_ptr<RandomNumberGenerator> functionRNG = make_shared<RandomNumberGenerator>();

	return
	{
		{
			"sin",
			new SineFunction()
		},
		{
			"cos",
			new CosineFunction()
		},
		{
			"tan",
			new TangentFunction()
		},
		{
			"arcsin",
			new ArcsineFunction()
		},
		{
			"arccos",
			new ArccosineFunction()
		},
		{
			"arctan",
			new ArctangentFunction()
		},
		{
			"sinh",
			new HypSineFunction()
		},
		{
			"cosh",
			new HypCosineFunction()
		},
		{
			"tanh",
			new HypTangentFunction()
		},
		{
			"arcsinh",
			new HypArcsineFunction()
		},
		{
			"arccosh",
			new HypArccosineFunction()
		},
		{
			"arctanh",
			new HypArctangentFunction()
		},
		{
			"sqrt",
			new SqrtFunction()
		},
		{
			"ln",
			new LnFunction()
		},
		{
			"log",
			new LogFunction()
		},
		{
			"logbase",
			new LogBaseFunction()
		},
		{
			"ans",
			new AnsFunction()
		},
		{
			"deriv",
			new DerivFunction()
		},
		{
			"integral",
			new IntegralFunction()
		},
		{
			"solve",
			new SolveFunction()
		},
		{
			"ceil",
			new CeilFunction()
		},
		{
			"floor",
			new FloorFunction()
		},
		{
			"round",
			new RoundFunction()
		},
		{
			"primeFct",
			new PrimeFctFunction()
		},
		{
			"isPrime",
			new IsPrimeFunction()
		},
		{
			"gcd",
			new GCDFunction()
		},
		{
			"lcm",
			new LCMFunction()
		},
		{
			"rand",
			new RandFunction(functionRNG)
		},
		{
			"randInt",
			new RandIntFunction(functionRNG)
		},
		{
			"seq",
			new SeqFunction()
		},
		{
			"foreach",
			new ForeachFunction()
		},
		{
			"getIndex",
			new GetIndexFunction()
		},
		{
			"setIndex",
			new SetIndexFunction()
		},
		{
			"length",
			new LengthFunction()
		},
		{
			"concat",
			new ConcatFunction()
		},
		{
			"sublist",
			new SublistFunction()
		},
		{
			"filter",
			new FilterFunction()
		},
		{
			"avg",
			new AvgFunction()
		},
		{
			"sum",
			new SumFunction()
		},
		{
			"product",
			new ProductFunction()
		},
		{
			"stddev",
			new StandardDevFunction()
		},
		{
			"variance",
			new VarianceFunction()
		},
		{
			"max",
			new MaxFunction()
		},
		{
			"min",
			new MinFunction()
		},
		{
			"and",
			new AndFunction()
		},
		{
			"or",
			new OrFunction()
		},
		{
			"not",
			new NotFunction()
		},
		{
			"lte",
			new LessThanEqualFunction()
		},
		{
			"lt",
			new LessThanFunction()
		},
		{
			"gte",
			new GreaterThanEqualFunction()
		},
		{
			"gt",
			new GreaterThanFunction()
		},
		{
			"eq",
			new EqualityFunction()
		},
		{
			"ne",
			new InequalityFunction()
		},
		{
			"if",
			new IfFunction()
		}
	};
}

#endif