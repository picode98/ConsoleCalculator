#pragma once

#ifndef PARSE_ARITHMETIC_H
#define PARSE_ARITHMETIC_H

#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <set>
#include <variant>

#ifndef MULTIPRECISION
#include <random>
#include <chrono>
#endif

#include "GlobalBuildVariables.h"
#include "CalculatorNumericTypes.h"
#include "CalcObj.h"
#include "ParserOperators.h"
#include "ParserMathematicalFunctions.h"
#include "ParserErrors.h"
#include "ParsingSettings.h"
#include "FunctionSignature.h"
#include "BuiltInFunctionTypes.h"
#include "BuiltInConstants.h"


/*
#ifndef MULTIPRECISION
#include "Gamma.h"
#endif
*/

/* #include <boost/math/constants/constants.hpp>
#include <boost/random.hpp>

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include "CalcObj.h"

#define UNSIGNED_BITS 128

using boost::multiprecision::cpp_dec_float_50;
using namespace boost::multiprecision;

typedef cpp_dec_float_50 calcFloat;
typedef number< cpp_int_backend<0, 0, signed_magnitude> > calcSignedInt;
typedef number< cpp_int_backend<UNSIGNED_BITS, UNSIGNED_BITS, unsigned_magnitude> > calcUnsignedInt; */

using namespace std;

class ExpressionParser
{
public:
	static const string RESERVED_NAMES[];
	static const unsigned RESERVED_NAMES_SIZE;

	struct FunctionDefinition
	{
		vector<string> params;
		string fnExpression;
	};

	class ParsingContext
	{
	private:
		std::map<std::string, CalcObj> valueMap;
		std::multimap<std::string, FunctionSignature> functionNameMap;
		std::map<FunctionSignature, FunctionDefinition> functionMap;
		std::vector<CalcObj> resultHistory;

		const ParsingContext* inheritedContext = nullptr;
	public:
		/* class VariableIterator
		{
		private:
			bool isEndIterator;
			std::map<std::string, CalcObj>::iterator mapIterator;
		public:
			CalcObj& operator*();
			VariableIterator& operator++();
			bool operator==(const VariableIterator& otherIterator);
			bool operator!=(const VariableIterator& otherIterator);
		};

		class FunctionIterator
		{
		private:
			bool isEndIterator;
			std::multimap<std::string, functionDefinition>::iterator mapIterator;
		public:
			functionDefinition& operator*();
			FunctionIterator& operator++();
			bool operator==(const FunctionIterator& otherIterator);
			bool operator!=(const FunctionIterator& otherIterator);
		}; */

		class VariableIteratorConst
		{
			friend ParsingContext;
		private:
			bool isEndIterator;
			const ParsingContext* parsingContext;
			const ParsingContext* baseContext;
			std::map<std::string, CalcObj>::const_iterator constMapIterator;
		public:
			VariableIteratorConst(bool isEndIterator, const ParsingContext* parsingContext, const ParsingContext* baseContext, std::map<std::string, CalcObj>::const_iterator variableMapIterator);
			string getVariableName() const;
			const CalcObj& operator*() const;
			VariableIteratorConst& operator++();
			bool operator==(const VariableIteratorConst& otherIterator);
			bool operator!=(const VariableIteratorConst& otherIterator);
		};

		class FunctionIteratorConst
		{
		private:
			bool isEndIterator;
			const ParsingContext* parsingContext;
			const ParsingContext* baseContext;
			std::map<FunctionSignature, FunctionDefinition>::const_iterator constMapIterator;
		public:
			FunctionIteratorConst(bool isEndIterator, const ParsingContext* parsingContext, const ParsingContext* baseContext, std::map<FunctionSignature, FunctionDefinition>::const_iterator functionMapIterator);
			FunctionSignature getFunctionSignature();
			const FunctionDefinition& operator*();
			FunctionIteratorConst& operator++();
			bool operator==(const FunctionIteratorConst& otherIterator);
			bool operator!=(const FunctionIteratorConst& otherIterator);
		};

		ParsingContext();
		ParsingContext(const std::map<std::string, CalcObj>& valueMap, const std::map<FunctionSignature, FunctionDefinition>& functionMap, const std::vector<CalcObj>& resultHistory);
		ParsingContext(const ParsingContext* inheritedContext);

		// VariableIterator beginVariable() const;
		// VariableIterator endVariable() const;
		VariableIteratorConst beginVariableConst() const;
		VariableIteratorConst endVariableConst() const;
		FunctionIteratorConst beginFunctionConst() const;
		FunctionIteratorConst endFunctionConst() const;

		void setVariable(const string& newVariableName, const CalcObj& newVariable);
		void setFunction(const FunctionSignature& newFunctionSignature, const FunctionDefinition& newFunction);

		bool variableExists(const string& variableName) const;
		bool functionExists(const string& functionName) const;
		bool functionExists(const FunctionSignature& functionSignature) const;
		VariableIteratorConst findVariable(const string& variableName) const;
		FunctionIteratorConst findFunction(const FunctionSignature& functionSignature) const;

		unsigned getNumVariables() const;
		unsigned getNumFunctions() const;

		bool deleteVariableFromCurrent(const string& variableName);
		bool deleteFunctionFromCurrent(const FunctionSignature& functionSignature);
	};

	static bool isVarChar(char ch);

	ParsingSettings parsingSettings;
	vector<CalcObj> resultHistory;
	std::map<std::string, BuiltInFunctionDefinition*> builtInFunctions;
private:
	struct FnResult
	{
		bool validFn;
		bool validDomain;
		std::string specificErrorStr;
		CalcObj result;
	};

	bool parseListString(const std::string& listStr, CalcObj& parsedObj, const ParsingContext& parsingContext) const;

	bool isValidImplicitTerm(string expression, unsigned basePosition, const ParsingContext& parsingContext) const;

	bool parseOperators(string expression,
		CalcObj& result,
		const ParsingContext& parsingContext,
		std::string parentExpression) const;

	// CalcObj parseMultDivOperator(char currentOperation, CalcObj currentTotal, std::string currentArg, std::string expression, const std::map<std::string, CalcObj>& valueMap, const std::multimap<std::string, functionDefinition> functionMap, const parseSettings& settings, const std::vector<CalcObj>& resultHistory = vector<CalcObj>());
	FnResult evalFn(std::string expression, const ParsingContext& parsingContext) const;
	// multimap<string, functionDefinition>::const_iterator findFunctionConst(const multimap<string, functionDefinition>& functionMap, string functionName, unsigned numParams);
	// multimap<string, functionDefinition>::iterator findFunction(multimap<string, functionDefinition>& functionMap, string functionName, unsigned numParams);
	static std::string::size_type findClosingChar(char openStr, char closeStr, int openCharIndex, std::string expression);
	calcFloat deriv(std::string expression,
		std::string variable,
		calcFloat value,
		calcFloat delta,
		const ParsingContext& parsingContext,
		std::string parentExpression = "",
		unsigned derivNum = 1) const;

	/* calcFloat integral(std::string expression,
		std::string variable,
		calcFloat lower,
		calcFloat upper,
		calcFloat delta,
		const ParsingContext& parsingContext,
		std::string parentExpression = ""); */

	calcFloat gaussIntegral(std::string expression,
		std::string variable,
		calcFloat lower,
		calcFloat upper,
		unsigned estimationPoints,
		const ParsingContext& parsingContext,
		std::string parentExpression = "") const;
	static calcFloat scaleNum(calcFloat oldMin, calcFloat oldMax, calcFloat newMin, calcFloat newMax, calcFloat value);

	void evalSeq(std::vector<CalcObj>& result,
		std::string expression,
		std::string variable,
		calcFloat lower,
		calcFloat upper,
		const ParsingContext& parsingContext,
		std::string parentExpression = "",
		bool automaticallyNegateDelta = true,
		calcFloat delta = 1.0) const;

	class SingleVariableParsedFunction : public ParserMathematicalFunctions::SingleVariableFunction
	{
	private:
		std::string expression,
			parentExpression,
			variable,
			typeErrorStr;

		const ExpressionParser* parser;
		const ParsingContext* parsingContext;

	public:
		SingleVariableParsedFunction(std::string expression, std::string parentExpression, std::string variable, std::string typeErrorStr,
			const ExpressionParser* parser, const ParsingContext* parsingContext);

		calcFloat evaluate(calcFloat value) const;
	};

	class LegendrePolynomial : public ParserMathematicalFunctions::SingleVariableFunction
	{
	private:
		unsigned polynomialOrder;
		vector<calcFloat> summationFactors;
		long calculationPrecision;
	public:
		LegendrePolynomial(unsigned polynomialOrder);

		calcFloat evaluate(calcFloat value) const;
	};

	static bool isIntegerNumber(string expression);
	static bool isDecimalNumber(std::string expression);
public:
	void bisectionSolve(std::string expression,
		std::string variable,
		calcFloat lBound,
		calcFloat hBound,
		std::vector<calcFloat>& solutions,
		unsigned numIntervals,
		const ParsingContext& parsingContext,
		std::string parentExpression = "") const;

	CalcObj parseArithmetic(std::string expression,
		const ParsingContext& parsingContext,
		std::string parentExpression = "") const;
};

// Ugly hack: Identical derived class created for the ExpressionParser::ParsingContext
// class because C++ doesn't support forward declarations for nested classes (see
// BuiltInFunctionTypes.h).
class ParsingContext_Export : public ExpressionParser::ParsingContext
{
	using ParsingContext::ParsingContext;
};
#endif