#pragma once

#ifndef _CONSOLE_CALCULATOR_H
#define _CONSOLE_CALCULATOR_H

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <codecvt>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
// #include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

#define NOMINMAX
#include <Windows.h>

#include "GlobalBuildVariables.h"
#include "CalculatorNumericTypes.h"
#include "parseArithmetic.h"
#include "CalcObj.h"
#include "StringUtils.h"

using namespace std;

class ConsoleCalculator
{
private:
	static const string ANGLE_MODE_KEY,
						DEC_FRAC_KEY,
						OUTPUT_MODE_KEY,
						PRECISION_MODE_KEY,
						CONSTANTS_KEY,
						CONSTANT_KEY_PATH,
						SAVED_FUNCTIONS_KEY,
						SAVED_FUNCTION_KEY_PATH,
						CONFIG_VERSION_KEY;

	static const string CONFIG_VERSION;

#ifdef MULTIPRECISION
	static const unsigned DEFAULT_PRECISION = 50,
						  GUARD_DIGITS = 3;
#else
	const unsigned DEFAULT_PRECISION = numeric_limits<calcFloat>::digits10;
#endif
	enum outputMode { AUTO_MODE, SCIENTIFIC_MODE, FIXED_MODE };

	class EnumConversionException : public exception
	{
		virtual const char* what() const throw()
		{
			return "Enum conversion exception";
		}
	};

	template <typename KeyType, typename ValueType, typename ExceptionType>
	static ValueType mapLookup(const map<KeyType, ValueType>& conversionMap, KeyType value)
	{
		map<KeyType, ValueType>::const_iterator lookupResult = conversionMap.find(value);

		if (lookupResult != conversionMap.cend())
		{
			return (lookupResult->second);
		}
		else
		{
			throw ExceptionType();
		}
	}

	template <typename KeyType, typename ValueType>
	static map<ValueType, KeyType> invertOneToOneMap(const map<KeyType, ValueType>& origMap)
	{
		map<ValueType, KeyType> resultMap;

		for (map<KeyType, ValueType>::const_iterator i = origMap.cbegin(); i != origMap.cend(); i++)
		{
			resultMap.insert(pair<ValueType, KeyType>(i->second, i->first));
		}

		return resultMap;
	}

	static const map<ExpressionParser::angleMode, string> ANGLE_MODE_TO_SHORT_ANGLE_STRING,
														  ANGLE_MODE_TO_LONG_ANGLE_STRING;
	static const map<string, ExpressionParser::angleMode> SHORT_ANGLE_STRING_TO_ANGLE_MODE,
														  LONG_ANGLE_STRING_TO_ANGLE_MODE;

	static const map<outputMode, string> OUTPUT_MODE_TO_SHORT_STRING,
										 OUTPUT_MODE_TO_LONG_STRING;
	static const map<string, outputMode> SHORT_STRING_TO_OUTPUT_MODE,
										 LONG_STRING_TO_OUTPUT_MODE;

	static const map<decimalFractionMode, string> DEC_FRAC_MODE_TO_STRING;
	static const map<string, decimalFractionMode> STRING_TO_DEC_FRAC_MODE;


	template <typename EnumType>
	inline string enumToString(EnumType value, const map<EnumType, string>& conversionMap)
	{
		return mapLookup<EnumType, string, EnumConversionException>(conversionMap, value);
	}

	template <typename EnumType>
	inline EnumType stringToEnum(string value, const map<string, EnumType>& conversionMap)
	{
		return mapLookup<string, EnumType, EnumConversionException>(conversionMap, value);
	}

	/*
	static inline string getShortAngleModeString(ExpressionParser::angleMode value)
	{
		return mapLookup<ExpressionParser::angleMode, string, EnumConversionException>(ANGLE_MODE_TO_SHORT_ANGLE_STRING, value);
	}

	static inline string getLongAngleModeString(ExpressionParser::angleMode value)
	{
		return mapLookup<ExpressionParser::angleMode, string, EnumConversionException>(ANGLE_MODE_TO_LONG_ANGLE_STRING, value);
	}

	static inline ExpressionParser::angleMode parseShortAngleModeString(string value)
	{
		return mapLookup<string, ExpressionParser::angleMode, EnumConversionException>(SHORT_ANGLE_STRING_TO_ANGLE_MODE, value);
	}

	static inline ExpressionParser::angleMode parseLongAngleModeString(string value)
	{
		return mapLookup<string, ExpressionParser::angleMode, EnumConversionException>(LONG_ANGLE_STRING_TO_ANGLE_MODE, value);
	}
	*/

	/*
	class outputMode
	{
	public:
		enum outputModeValue {AUTO_MODE, SCIENTIFIC_MODE, FIXED_MODE};
		outputModeValue value;

		outputMode(string strValue, bool shortFormat);
		outputMode(outputModeValue startValue);

		void setFromString(string strValue, bool shortFormat);
		string getString(bool shortFormat);
	};
	*/

	enum UserMessageType { INFORMATION_MESSAGE, WARNING_MESSAGE, ERROR_MESSAGE };

	vector<string> inputHistory;

	ExpressionParser calculatorParser;

	ExpressionParser::ParsingContext baseParsingContext;

	set<string> constantNames;
	set<ExpressionParser::FunctionSignature> savedFunctionSignatures;

	outputMode calcOutputMode;
	unsigned outputPrecision;

	wstring helpFilePath,
			configFilePath;


	static string functionToString(ExpressionParser::FunctionSignature functionSignature, ExpressionParser::FunctionDefinition fnDef);
	static void setUserFlags(ostream& outStream, outputMode mode);
	static pair<ExpressionParser::FunctionSignature, ExpressionParser::FunctionDefinition> stringToFunction(string functionStr);
	// static vector<string> &split(const string &s, char delim, vector<string> &elems);
	// static vector<std::string> split(const string &s, char delim);
	static void toClipboard(const string &s);
	// static int getConsoleBufferWidth();
	static void printUserMessage(UserMessageType messageType, wstring message, wostream& outputStream, wostream& errorStream);
	static int compareVersionStrings(string firstString, string secondString);

	bool validVarName(string name, bool variableName, bool constName);
	bool validFunctionSignature(const ExpressionParser::FunctionSignature& functionSignature, bool savedFunction);
	bool parseUserStringAux(string userString, bool csvFormat = false);
	void saveConfiguration();
public:
	ConsoleCalculator(wstring helpFilePath);

	void setConfiguration(wstring configFilePath);
	void parseUserString(string userString, bool csvFormat = false);
	void runPromptLoop();
};
#endif