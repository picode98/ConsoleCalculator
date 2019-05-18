#include "ParserOperators.h"

string ParserOperators::AdditionOperator::getOperatorStr()
{
	return "+";
}

void ParserOperators::AdditionOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base += value;
}

string ParserOperators::SubtractionOperator::getOperatorStr()
{
	return "-";
}

void ParserOperators::SubtractionOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base -= value;
}

string ParserOperators::MultiplicationOperator::getOperatorStr()
{
	return "*";
}

void ParserOperators::MultiplicationOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base *= value;
}

string ParserOperators::DivisionOperator::getOperatorStr()
{
	return "/";
}

void ParserOperators::DivisionOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base /= value;
}

string ParserOperators::ModuloOperator::getOperatorStr()
{
	return "%";
}

void ParserOperators::ModuloOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = fmod(base, value);
}

string ParserOperators::PowerOperator::getOperatorStr()
{
	return "^";
}

void ParserOperators::PowerOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = pow(value, base);
}

string ParserOperators::LessThanOperator::getOperatorStr()
{
	return "<";
}

void ParserOperators::LessThanOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = calcObj(boolToFloat(base < value));
}

string ParserOperators::LessThanOrEqualOperator::getOperatorStr()
{
	return "<=";
}

void ParserOperators::LessThanOrEqualOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = calcObj(boolToFloat(base <= value));
}

string ParserOperators::GreaterThanOperator::getOperatorStr()
{
	return ">";
}

void ParserOperators::GreaterThanOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = calcObj(boolToFloat(base > value));
}

string ParserOperators::GreaterThanOrEqualOperator::getOperatorStr()
{
	return ">=";
}

void ParserOperators::GreaterThanOrEqualOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = calcObj(boolToFloat(base >= value));
}

string ParserOperators::EqualityOperator::getOperatorStr()
{
	return "=";
}

void ParserOperators::EqualityOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = calcObj(boolToFloat(base == value));
}

string ParserOperators::InequalityOperator::getOperatorStr()
{
	return "!=";
}

void ParserOperators::InequalityOperator::applyOperator(calcObj& base, const calcObj& value)
{
	base = calcObj(boolToFloat(base != value));
}

ParserOperators::UnaryOperator::OperatorStrMode ParserOperators::NegationOperator::getOperatorStrMode()
{
	return UnaryOperator::OperatorStrMode::PREFIX_MODE;
}

string ParserOperators::NegationOperator::getOperatorStr()
{
	return "-";
}

void ParserOperators::NegationOperator::applyOperator(calcObj& base)
{
	base = -base;
}

ParserOperators::UnaryOperator::OperatorStrMode ParserOperators::FactorialOperator::getOperatorStrMode()
{
	return UnaryOperator::OperatorStrMode::POSTFIX_MODE;
}

string ParserOperators::FactorialOperator::getOperatorStr()
{
	return "!";
}

void ParserOperators::FactorialOperator::applyOperator(calcObj& base)
{
	calcObj result = ParserMathematicalFunctions::factorial(base.getVerifiedFloat("Factorial type error"));

	if (result == -1)
	{
		// throw exception("Domain error in factorial expression.");
		throw DomainError("factorial expression");
	}
	else
	{
		base = result;
	}
}

vector<ParserOperators::PrecedenceLevel> ParserOperators::getOperatorLists()
{
	static vector<ParserOperators::PrecedenceLevel> operatorLists;

	if (operatorLists.empty())
	{
		operatorLists.emplace_back(ParserOperators::PrecedenceLevel());
		operatorLists.back().associativity = ParserOperators::PrecedenceLevel::OperatorAssociativity::LEFT_TO_RIGHT;
		operatorLists.back().operatorType = ParserOperators::PrecedenceLevel::OperatorType::BINARY_TYPE;

		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::LessThanOperator());
		operatorLists.back().binaryOperators.back()->overrides.emplace_back(ParserOperators::BinaryOperatorParseOverride());
		operatorLists.back().binaryOperators.back()->overrides.back().overridingOperator = new ParserOperators::LessThanOrEqualOperator();
		operatorLists.back().binaryOperators.back()->overrides.back().precedenceLevel = (operatorLists.size() - 1);

		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::LessThanOrEqualOperator());

		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::GreaterThanOperator());
		operatorLists.back().binaryOperators.back()->overrides.emplace_back(ParserOperators::BinaryOperatorParseOverride());
		operatorLists.back().binaryOperators.back()->overrides.back().overridingOperator = new ParserOperators::GreaterThanOrEqualOperator();
		operatorLists.back().binaryOperators.back()->overrides.back().precedenceLevel = (operatorLists.size() - 1);

		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::GreaterThanOrEqualOperator());

		operatorLists.emplace_back(ParserOperators::PrecedenceLevel());
		operatorLists.back().associativity = ParserOperators::PrecedenceLevel::OperatorAssociativity::LEFT_TO_RIGHT;
		operatorLists.back().operatorType = ParserOperators::PrecedenceLevel::OperatorType::BINARY_TYPE;
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::EqualityOperator());
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::InequalityOperator());

		operatorLists.emplace_back(ParserOperators::PrecedenceLevel());
		operatorLists.back().associativity = ParserOperators::PrecedenceLevel::OperatorAssociativity::LEFT_TO_RIGHT;
		operatorLists.back().operatorType = ParserOperators::PrecedenceLevel::OperatorType::BINARY_TYPE;
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::AdditionOperator());
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::SubtractionOperator());

		operatorLists.emplace_back(ParserOperators::PrecedenceLevel());
		operatorLists.back().associativity = ParserOperators::PrecedenceLevel::OperatorAssociativity::LEFT_TO_RIGHT;
		operatorLists.back().operatorType = ParserOperators::PrecedenceLevel::OperatorType::BINARY_TYPE;
		operatorLists.back().supportsImplicitMult = true;
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::MultiplicationOperator());
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::DivisionOperator());
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::ModuloOperator());

		operatorLists.emplace_back(ParserOperators::PrecedenceLevel());
		operatorLists.back().associativity = ParserOperators::PrecedenceLevel::OperatorAssociativity::LEFT_TO_RIGHT;
		operatorLists.back().operatorType = ParserOperators::PrecedenceLevel::OperatorType::UNARY_TYPE;
		operatorLists.back().unaryOperators.emplace_back(new ParserOperators::NegationOperator());

		operatorLists.emplace_back(ParserOperators::PrecedenceLevel());
		operatorLists.back().associativity = ParserOperators::PrecedenceLevel::OperatorAssociativity::RIGHT_TO_LEFT;
		operatorLists.back().operatorType = ParserOperators::PrecedenceLevel::OperatorType::BINARY_TYPE;
		operatorLists.back().binaryOperators.emplace_back(new ParserOperators::PowerOperator());

		operatorLists.emplace_back(ParserOperators::PrecedenceLevel());
		operatorLists.back().associativity = ParserOperators::PrecedenceLevel::OperatorAssociativity::LEFT_TO_RIGHT;
		operatorLists.back().operatorType = ParserOperators::PrecedenceLevel::OperatorType::UNARY_TYPE;
		operatorLists.back().unaryOperators.emplace_back(new ParserOperators::FactorialOperator());


		/* binaryOperatorLists.emplace_back(vector<string>());
		binaryOperatorLists[1].emplace_back("*");
		binaryOperatorLists[1].emplace_back("/");
		binaryOperatorLists[1].emplace_back("%");

		binaryOperatorLists.emplace_back(vector<string>());
		binaryOperatorLists[2].emplace_back("^"); */
	}

	return operatorLists;
}