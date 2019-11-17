#include "stdafx.h"
#include "CalculatorNumericTypes.h"
#include "CalcParser.h"


using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

using namespace std;

namespace ConsoleCalculatorTests
{
	[TestClass]
	public ref class ParserTests
	{
	public:
		CalcParser* testParser;
		CalcParser::ParsingContext* testContext;
		const unsigned testDigits = 50;

		ParserTests()
		{
#ifdef MULTIPRECISION
			mpfr::mpreal::set_default_prec(mpfr::digits2bits(testDigits));
#endif
			testParser = new CalcParser();
			testContext = new ExpressionParser::ParsingContext();
		}

#ifdef MULTIPRECISION
		inline static bool isWithinTolerance(const calcFloat& resultValue, const calcFloat& expectedValue, double epsilonIncrement)
		{
			return (abs(resultValue - expectedValue) <= (numeric_limits<mpfr::mpreal>::epsilon(expectedValue) * epsilonIncrement));
		}
#else
		inline static bool isWithinTolerance(const calcFloat& resultValue, const calcFloat& expectedValue, double epsilonIncrement)
		{
			return (abs(boost::math::float_distance(resultValue, expectedValue)) <= epsilonIncrement);
		}
#endif
		inline static bool isWithinTolerance(const calcFloat& resultValue, const calcFloat& expectedValue)
		{
			return isWithinTolerance(resultValue, expectedValue, 64.0);
		}

		/*
		inline static bool isWithinTolerance(const calcFloat& resultValue, const calcFloat& expectedValue, const calcFloat& tolerance)
		{
			return (abs(resultValue - expectedValue) <= tolerance);
		}
		*/

		bool checkFloatResult(string expression, string expectedResult, bool parseExpected, bool exactEquality, CalcParser::ParsingContext* context, double epsilonIncrement)
		{
			calcFloat expressionResult = this->testParser->parseArithmetic(expression, *context).getVerifiedFloat("Unit test type error");
			calcFloat expectedResultFloat;

			if (parseExpected)
			{
				expectedResultFloat = this->testParser->parseArithmetic(expectedResult, *context).getVerifiedFloat("Unit test type error");
			}
			else
			{
				stringToFloat(expectedResult, expectedResultFloat);
			}

			if (exactEquality)
			{
				return (expressionResult == expectedResultFloat);
			}
			else
			{
				return isWithinTolerance(expressionResult, expectedResultFloat, epsilonIncrement);
			}
		}

		inline bool checkFloatResult(string expression, string expectedResult, bool parseExpected, bool exactEquality, CalcParser::ParsingContext* context)
		{
			return checkFloatResult(expression, expectedResult, parseExpected, exactEquality, context, 64.0);
		}

		inline bool checkFloatResult(string expression, string expectedResult, bool parseExpected, bool exactEquality)
		{
			return checkFloatResult(expression, expectedResult, parseExpected, exactEquality, this->testContext);
		}


		[TestMethod]
		void AddSubtractTest()
		{
			Assert::IsTrue(checkFloatResult("5+3", "8", false, true));

			Assert::IsTrue(checkFloatResult("11-7", "4", false, true));

			Assert::IsTrue(checkFloatResult("16-19+20+25-41", "1", false, true));
		}

		[TestMethod]
		void MultDivTest()
		{
			Assert::IsTrue(checkFloatResult("5*3", "15", false, true));

			Assert::IsTrue(checkFloatResult("120.5/0.5", "241", false, true));

			Assert::IsTrue(checkFloatResult("16/19*19/40*0.5", "0.2", false, false));
		}

		[TestMethod]
		void ExponentOperatorTest()
		{
			Assert::IsTrue(checkFloatResult("5^3", "125", false, true));

			Assert::IsTrue(checkFloatResult("(5^3^(-2))^9", "5", false, false));
		}

		[TestMethod]
		void ImplicitMultTest()
		{
			ExpressionParser::ParsingContext multTestContext;
			multTestContext.setFunction(FunctionSignature{ "test", 1 }, ExpressionParser::FunctionDefinition{ {"x"}, "x^2" });
			Assert::IsTrue(checkFloatResult("8.5(6)cos(pi/12)2sin(pi/12)(-7)4", "-714", false, false));

			Assert::IsTrue(checkFloatResult("-0.5(10)6test(1/2)test(2)+100(0.1)^2", "-29", false, true, &multTestContext));

			vector<calcFloat> resultVector;
			CalcObj compareObj, result;
			calcFloat lastValue;
			stringToFloat("3.5", lastValue);

			resultVector.emplace_back(1);
			resultVector.push_back(lastValue);

			compareObj.set_list(resultVector);

			result = testParser->parseArithmetic("5{1,3.5}2(0.1)", *testContext);
			Assert::IsTrue(result == compareObj);
		}

		[TestMethod]
		void RelationalOperatorTest()
		{
			Assert::IsTrue(checkFloatResult("5.5<4", "0", false, true));

			Assert::IsTrue(checkFloatResult("10.5>4.3", "1", false, true));

			Assert::IsTrue(checkFloatResult("5.5<=5.5", "1", false, true));

			Assert::IsTrue(checkFloatResult("5.5>=4", "1", false, true));

			Assert::IsTrue(checkFloatResult("4.25=4.25", "1", false, true));

			Assert::IsTrue(checkFloatResult("{5.5,6,2.4}={11/2,2*3,2+2/5}", "1", false, true));

			Assert::IsTrue(checkFloatResult("{5.5,6,2.4}!={11/2,2*3,2+3/5}", "1", false, true));
		}

		[TestMethod]
		void TrigFunctionTest()
		{
			testParser->parsingSettings.parseAngleMode = ParsingSettings::radians;
			
			Assert::IsTrue(checkFloatResult("sin(pi/6)", "0.5", false, false));

			Assert::IsTrue(checkFloatResult("cos(pi/3)", "0.5", false, false));

			Assert::IsTrue(checkFloatResult("tan(pi/4)", "1", false, false));

			Assert::IsTrue(checkFloatResult("arcsin(0.5)", "pi/6", true, false));

			Assert::IsTrue(checkFloatResult("arccos(0.5)", "pi/3", true, false));

			Assert::IsTrue(checkFloatResult("arctan(1)", "pi/4", true, false));
		}

		[TestMethod]
		void LogTest()
		{
			Assert::IsTrue(checkFloatResult("ln(1)", "0", false, true));

			Assert::IsTrue(checkFloatResult("ln(e^2)", "2", false, true));

			try
			{
				this->testParser->parseArithmetic("ln(-1)", *(this->testContext));

				Assert::Fail("The ln function failed to throw a domain error for negative values.");
			}
			catch (DomainError)
			{

			}

			Assert::IsTrue(checkFloatResult("log(1)", "0", false, true));

			Assert::IsTrue(checkFloatResult("log(1000)", "3", false, true));

			try
			{
				this->testParser->parseArithmetic("log(-1)", *(this->testContext));

				Assert::Fail("The log function failed to throw a domain error for negative values.");
			}
			catch (DomainError)
			{

			}

			Assert::IsTrue(checkFloatResult("logbase(1,2)", "0", false, true));

			Assert::IsTrue(checkFloatResult("logbase(81,9)", "2", false, true));

			try
			{
				this->testParser->parseArithmetic("logbase(-1,5)", *(this->testContext));

				Assert::Fail("The logbase function failed to throw a domain error for negative values.");
			}
			catch (DomainError)
			{

			}

			try
			{
				this->testParser->parseArithmetic("logbase(5,0)", *(this->testContext));

				Assert::Fail("The logbase function failed to throw a domain error for zero bases.");
			}
			catch (DomainError)
			{

			}

			try
			{
				this->testParser->parseArithmetic("logbase(8,-2)", *(this->testContext));

				Assert::Fail("The logbase function failed to throw a domain error for negative bases.");
			}
			catch (DomainError)
			{

			}
		}

		[TestMethod]
		void AnsFunctionTest()
		{
			this->testParser->resultHistory.push_back(CalcObj("120", true));

			Assert::IsTrue(checkFloatResult("ans(1)", "120", false, true));

			try
			{
				this->testParser->parseArithmetic("ans(-1)", *(this->testContext));

				Assert::Fail("The ans function failed to throw a domain error for negative indices.");
			}
			catch (DomainError)
			{

			}

			try
			{
				this->testParser->parseArithmetic("ans(0)", *(this->testContext));

				Assert::Fail("The ans function failed to throw a domain error for zero indices.");
			}
			catch (DomainError)
			{

			}

			try
			{
				this->testParser->parseArithmetic("ans(2)", *(this->testContext));

				Assert::Fail("The ans function failed to throw a domain error for indices that are too large.");
			}
			catch (DomainError)
			{

			}

			this->testParser->resultHistory.clear();
		}

		[TestMethod]
		void DerivFunctionTest()
		{
			Assert::IsTrue(checkFloatResult("deriv(x^2,x,4.5)", "9", false, false, this->testContext, 16384.0));

			try
			{
				this->testParser->parseArithmetic("deriv(x^2,x,4.5,-1)", *(this->testContext));

				Assert::Fail("The deriv function failed to throw a domain error for negative derivative numbers.");
			}
			catch (DomainError)
			{

			}
		}

		[TestMethod]
		void IntegralFunctionTest()
		{
			this->testParser->parsingSettings.parseAngleMode = ParsingSettings::radians;
			Assert::IsTrue(checkFloatResult("integral(sin(x),x,0,pi)", "2", false, false));
		}

		[TestMethod]
		void SolveFunctionTest()
		{

		}

		[TestMethod]
		void ParsingContextVariableTest()
		{
			CalcObj result, compareObj;

			vector<calcFloat> resultVector;
			calcFloat firstValue;
			stringToFloat("15.75", firstValue);

			resultVector.push_back(firstValue);
			resultVector.push_back(calcFloat(-63));
			resultVector.push_back(calcFloat(21));

			compareObj.set_list(resultVector);

			ExpressionParser::ParsingContext firstInheritedContext = ExpressionParser::ParsingContext(testContext);

			firstInheritedContext.setVariable("firstContextOne", CalcObj("3.5", false));

			ExpressionParser::ParsingContext secondInheritedContext = ExpressionParser::ParsingContext(&firstInheritedContext);
			string secondVariableStr = "{4.5,-18,6}";

			secondInheritedContext.setVariable("firstContextTwo", CalcObj(secondVariableStr, false));

			result = testParser->parseArithmetic("firstContextOne*firstContextTwo", secondInheritedContext);
			Assert::IsTrue(result == compareObj);
		}

		/*
		[TestMethod]
		void FunctionBlacklistTest()
		{
			Assert::IsTrue(checkFloatResult("log(100)", "2", false, true));
			this->testParser->parsingSettings.functionBlacklist.insert("log");

			try
			{
				this->testParser->parseArithmetic("log(100)", *(this->testContext));

				Assert::Fail("Test parser failed to throw SyntaxError for blacklisted function");
			}
			catch(const SyntaxError&)
			{
				
			}
			finally
			{
				this->testParser->parsingSettings.functionBlacklist.clear();
			}
		}
		*/

		~ParserTests()
		{
			delete testParser;
			delete testContext;
		}
	};
}
