#pragma once

#include <string>
#include <vector>

#include "calcObj.h"
#include "ParserMathematicalFunctions.h"
#include "CalculatorNumericTypes.h"
#include "ParserErrors.h"

using namespace std;

namespace ParserOperators
{
	class UnaryOperator
	{
	public:
		enum OperatorStrMode { PREFIX_MODE, POSTFIX_MODE };

		virtual string getOperatorStr() = 0;
		virtual OperatorStrMode getOperatorStrMode() = 0;
		virtual void applyOperator(calcObj& base) = 0;
	};

	struct BinaryOperatorParseOverride;

	class BinaryOperator
	{
	public:
		virtual string getOperatorStr() = 0;
		virtual void applyOperator(calcObj& base, const calcObj& value) = 0;
		vector<BinaryOperatorParseOverride> overrides;
	};

	struct BinaryOperatorParseOverride
	{
		BinaryOperator* overridingOperator;
		unsigned precedenceLevel;
	};

	struct PrecedenceLevel
	{
		enum OperatorAssociativity { LEFT_TO_RIGHT, RIGHT_TO_LEFT };
		enum OperatorType { UNARY_TYPE, BINARY_TYPE };

		OperatorAssociativity associativity;
		OperatorType operatorType;

		vector<BinaryOperator*> binaryOperators;
		vector<UnaryOperator*> unaryOperators;

		bool supportsImplicitMult = false;
	};

	struct BinaryParsingBlock
	{
		BinaryOperator* terminatingOperator;
		string argument;
	};

	vector<PrecedenceLevel> getOperatorLists();

	class AdditionOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class SubtractionOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class MultiplicationOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class DivisionOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class ModuloOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class PowerOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class LessThanOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class LessThanOrEqualOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class GreaterThanOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};


	class GreaterThanOrEqualOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class EqualityOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class InequalityOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(calcObj& base, const calcObj& value);
	};

	class NegationOperator : public UnaryOperator
	{
	public:
		string getOperatorStr();
		OperatorStrMode getOperatorStrMode();
		void applyOperator(calcObj& base);
	};

	class FactorialOperator : public UnaryOperator
	{
	public:
		string getOperatorStr();
		OperatorStrMode getOperatorStrMode();
		void applyOperator(calcObj& base);
	};
}