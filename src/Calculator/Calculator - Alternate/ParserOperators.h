#pragma once

#include <string>
#include <vector>

#include "CalcObj.h"
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
		virtual void applyOperator(CalcObj& base) = 0;
	};

	struct BinaryOperatorParseOverride;

	class BinaryOperator
	{
	public:
		virtual string getOperatorStr() = 0;
		virtual void applyOperator(CalcObj& base, const CalcObj& value) = 0;
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
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class SubtractionOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class MultiplicationOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class DivisionOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class ModuloOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class PowerOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class LessThanOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class LessThanOrEqualOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class GreaterThanOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};


	class GreaterThanOrEqualOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class EqualityOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class InequalityOperator : public BinaryOperator
	{
	public:
		string getOperatorStr();
		void applyOperator(CalcObj& base, const CalcObj& value);
	};

	class NegationOperator : public UnaryOperator
	{
	public:
		string getOperatorStr();
		OperatorStrMode getOperatorStrMode();
		void applyOperator(CalcObj& base);
	};

	class FactorialOperator : public UnaryOperator
	{
	public:
		string getOperatorStr();
		OperatorStrMode getOperatorStrMode();
		void applyOperator(CalcObj& base);
	};
}