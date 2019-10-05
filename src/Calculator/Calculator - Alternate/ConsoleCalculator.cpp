#include "ConsoleCalculator.h"

const string ConsoleCalculator::ANGLE_MODE_KEY = "settings.evaluationSettings.angleMode",
			 ConsoleCalculator::DEC_FRAC_KEY = "settings.evaluationSettings.decFracMode",
			 ConsoleCalculator::OUTPUT_MODE_KEY = "settings.evaluationSettings.outputMode",
			 ConsoleCalculator::PRECISION_MODE_KEY = "settings.evaluationSettings.precision",
			 ConsoleCalculator::CONSTANTS_KEY = "settings.userConstants",
			 ConsoleCalculator::CONSTANT_KEY_PATH = "settings.userConstants.constant",
			 ConsoleCalculator::SAVED_FUNCTIONS_KEY = "settings.savedFunctions",
			 ConsoleCalculator::SAVED_FUNCTION_KEY_PATH = "settings.savedFunctions.function",
			 ConsoleCalculator::CONFIG_VERSION_KEY = "settings.configVersion";

const string ConsoleCalculator::CONFIG_VERSION = "1.0";

const wstring CONFIG_FILE_OPEN_ERROR_FORMAT = L"Could not open configuration file \"%s\".";

const map<ParsingSettings::angleMode, string> ConsoleCalculator::ANGLE_MODE_TO_SHORT_ANGLE_STRING = { { ParsingSettings::degrees, "DEG" },{ ParsingSettings::radians, "RAD" },{ ParsingSettings::gradians, "GRAD" } };
const map<ParsingSettings::angleMode, string> ConsoleCalculator::ANGLE_MODE_TO_LONG_ANGLE_STRING = { { ParsingSettings::degrees, "degrees" },{ ParsingSettings::radians, "radians" },{ ParsingSettings::gradians, "gradians" } };
const map<string, ParsingSettings::angleMode> ConsoleCalculator::SHORT_ANGLE_STRING_TO_ANGLE_MODE = invertOneToOneMap(ANGLE_MODE_TO_SHORT_ANGLE_STRING);
const map<string, ParsingSettings::angleMode> ConsoleCalculator::LONG_ANGLE_STRING_TO_ANGLE_MODE = invertOneToOneMap(ANGLE_MODE_TO_LONG_ANGLE_STRING);

const map<ConsoleCalculator::outputMode, string> ConsoleCalculator::OUTPUT_MODE_TO_SHORT_STRING = { {outputMode::AUTO_MODE, "AUTO"}, {outputMode::FIXED_MODE, "FIXED"}, {outputMode::SCIENTIFIC_MODE, "SCI"} };
const map<ConsoleCalculator::outputMode, string> ConsoleCalculator::OUTPUT_MODE_TO_LONG_STRING = { { outputMode::AUTO_MODE, "automatic" },{ outputMode::FIXED_MODE, "fixed point" },{ outputMode::SCIENTIFIC_MODE, "scientific" } };
const map<string, ConsoleCalculator::outputMode> ConsoleCalculator::SHORT_STRING_TO_OUTPUT_MODE = invertOneToOneMap(OUTPUT_MODE_TO_SHORT_STRING);
const map<string, ConsoleCalculator::outputMode> ConsoleCalculator::LONG_STRING_TO_OUTPUT_MODE = invertOneToOneMap(OUTPUT_MODE_TO_LONG_STRING);

const map<decimalFractionMode, string> ConsoleCalculator::DEC_FRAC_MODE_TO_STRING = { {decimalFractionMode::DECIMAL_VALUE, "DEC"}, { decimalFractionMode::FRACTION_VALUE, "FRAC" } };
const map<string, decimalFractionMode> ConsoleCalculator::STRING_TO_DEC_FRAC_MODE = invertOneToOneMap(DEC_FRAC_MODE_TO_STRING);

/*
#pragma region OutputMode
ConsoleCalculator::outputMode::outputMode(string strValue, bool shortFormat)
{
	this->setFromString(strValue, shortFormat);
}

ConsoleCalculator::outputMode::outputMode(outputModeValue startValue):
	value(startValue)
{

}

void ConsoleCalculator::outputMode::setFromString(string strValue, bool shortFormat)
{
	const string invalidStrValueMessage = "The string value provided for setting the outputMode object is invalid.";

	if(shortFormat)
	{
		if(strValue == "AUTO")
		{
			this->value = AUTO_MODE;
		}
		else if(strValue == "SCI")
		{
			this->value = SCIENTIFIC_MODE;
		}
		else if(strValue == "FIXED")
		{
			this->value = FIXED_MODE;
		}
		else
		{
			throw invalid_argument(invalidStrValueMessage);
		}
	}
	else
	{
		if(strValue == "automatic")
		{
			this->value = AUTO_MODE;
		}
		else if(strValue == "scientific")
		{
			this->value = SCIENTIFIC_MODE;
		}
		else if(strValue == "fixed point")
		{
			this->value = FIXED_MODE;
		}
		else
		{
			throw invalid_argument(invalidStrValueMessage);
		}
	}
}

string ConsoleCalculator::outputMode::getString(bool shortFormat)
{
	string strValue;

	switch(this->value)
	{
	case AUTO_MODE:
		strValue = (shortFormat ? "AUTO" : "automatic");
		break;
	case SCIENTIFIC_MODE:
		strValue = (shortFormat ? "SCI" : "scientific");
		break;
	case FIXED_MODE:
		strValue = (shortFormat ? "FIXED" : "fixed point");
		break;
	}

	return strValue;
}
#pragma endregion
*/

ConsoleCalculator::ConsoleCalculator(wstring helpFilePath):
	calcOutputMode(outputMode(outputMode::AUTO_MODE)),
	outputPrecision(ConsoleCalculator::DEFAULT_PRECISION),
	helpFilePath(helpFilePath)
{
#ifdef MULTIPRECISION
	mpfr::mpreal::set_default_prec(mpfr::digits2bits(outputPrecision));
#endif
	this->calculatorParser.parsingSettings.parseAngleMode = ParsingSettings::radians;
}

#pragma region StaticFunctions
	string ConsoleCalculator::functionToString(FunctionSignature functionSignature, ExpressionParser::FunctionDefinition fnDef)
	{
		string result = (functionSignature.functionName + '(');

		for(unsigned i = 0; i < fnDef.params.size(); i++)
		{
			result += fnDef.params[i];

			if(i < (fnDef.params.size() - 1))
			{
				result += ',';
			}
		}

		result += ("):" + fnDef.fnExpression);

		return result;
	}

	void ConsoleCalculator::setUserFlags(ostream& outStream, outputMode mode)
	{
		switch(mode)
		{
		case outputMode::AUTO_MODE:
			{
				ios::fmtflags currentFlags = outStream.flags();

				currentFlags -= (currentFlags & 0x1000);
				currentFlags -= (currentFlags & 0x2000);

				outStream.flags(currentFlags);

				break;
			}
		case outputMode::SCIENTIFIC_MODE:
			outStream << scientific;
			break;
		case outputMode::FIXED_MODE:
			outStream << fixed;
			break;
		}
	}

	pair<FunctionSignature, ExpressionParser::FunctionDefinition> ConsoleCalculator::stringToFunction(string functionStr)
	{
		pair<FunctionSignature, ExpressionParser::FunctionDefinition> resultFunction;

		vector<string> functionParts;
		vector<string> functionNameParts;
		StringUtils::split(functionStr, ":", functionParts, 2);
		StringUtils::split(functionParts[0], "(", functionNameParts);
		functionNameParts[1] = functionNameParts[1].substr(0, functionNameParts[1].size() - 1);

		vector<string> fnParams;
		StringUtils::split(functionNameParts[1], ",", fnParams);

		resultFunction.first.functionName = functionNameParts[0];
		resultFunction.first.numParams = fnParams.size();
		resultFunction.second.params = fnParams;
		resultFunction.second.fnExpression = functionParts[1];

		return resultFunction;
	}

	/*
	Original implementation of toClipboard by helios of cplusplus.com
	Post: http://www.cplusplus.com/forum/beginner/14349/#msg70160

	The active implementation below this one is based on this and MSDN documentation.

	void ConsoleCalculator::toClipboard(const string &s){
		OpenClipboard(0);
		EmptyClipboard();	
		HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size() + 1);
		if (!hg){
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg),s.c_str(),s.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT,hg);
		CloseClipboard();
		GlobalFree(hg);
	}
	*/

	void ConsoleCalculator::toClipboard(const string &value)
	{
		// Open the Windows clipboard and clear existing contents
		OpenClipboard(nullptr);
		EmptyClipboard();
		
		// Allocate global memory and obtain a lock to it
		HANDLE globalStrHandle = GlobalAlloc(GMEM_MOVEABLE, value.size() + 1);
		char* globalStrPtr = reinterpret_cast<char*>(GlobalLock(globalStrHandle));

#pragma warning(push)
#pragma warning(disable:4996)
		// Copy the string to the locked global memory
		strncpy(globalStrPtr, value.c_str(), value.size());
#pragma warning(pop)
		
		// Unlock the global memory and set its handle as the current clipboard data
		GlobalUnlock(globalStrHandle);
		globalStrPtr = nullptr;
		SetClipboardData(CF_TEXT, globalStrHandle);

		// Close the Windows clipboard
		CloseClipboard();
	}


	/* int ConsoleCalculator::getConsoleBufferWidth()
	{
		CONSOLE_SCREEN_BUFFER_INFO outputBufferInfo;
		BOOL successValue = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &outputBufferInfo);

		if (successValue)
		{
			return outputBufferInfo.dwSize.X;
		}
		else
		{
			return -1;
		}
	} */
#pragma endregion

void ConsoleCalculator::printUserMessage(UserMessageType messageType, wstring message, wostream& outputStream, wostream& errorStream)
{
	string resultString;

	switch (messageType)
	{
	case INFORMATION_MESSAGE:
		outputStream << "INFO: " << message << endl;
		break;
	case WARNING_MESSAGE:
		errorStream << "WARNING: " << message << endl;
		break;
	case ERROR_MESSAGE:
		errorStream << "ERROR: " << message << endl;
		break;
	default:
		outputStream << message << endl;
	}
}

int ConsoleCalculator::compareVersionStrings(string firstString, string secondString)
{
	unsigned firstIndex = 0, secondIndex = 0;
	int resultValue = 0;
	string firstCurrentString, secondCurrentString;

	while (firstIndex < firstString.size() && secondIndex < secondString.size() && resultValue == 0)
	{
		while (firstIndex < firstString.size() && firstString[firstIndex] != '.')
		{
			firstCurrentString += firstString[firstIndex];
			firstIndex++;
		}

		while (secondIndex < secondString.size() && secondString[secondIndex] != '.')
		{
			secondCurrentString += secondString[secondIndex];
			secondIndex++;
		}

		if (firstIndex < firstString.size())
		{
			firstIndex++;
		}

		if (secondIndex < secondString.size())
		{
			secondIndex++;
		}

		int firstValue = atoi(firstCurrentString.c_str()),
			secondValue = atoi(secondCurrentString.c_str());

		if (firstValue < secondValue)
		{
			resultValue = -1;
		}
		else if (firstValue > secondValue)
		{
			resultValue = 1;
		}

		firstCurrentString.clear();
		secondCurrentString.clear();
	}

	if (resultValue == 0)
	{
		if (firstIndex >= firstString.size() && secondIndex < secondString.size())
		{
			while (secondIndex < secondString.size() && resultValue == 0)
			{
				if (secondString[secondIndex] != '0' && secondString[secondIndex] != '.')
				{
					resultValue = -1;
				}

				secondIndex++;
			}
		}
		else if (firstIndex < firstString.size() && secondIndex >= secondString.size())
		{
			while (firstIndex < firstString.size() && resultValue == 0)
			{
				if (firstString[firstIndex] != '0' && firstString[firstIndex] != '.')
				{
					resultValue = 1;
				}

				firstIndex++;
			}
		}
	}

	return resultValue;
}

bool ConsoleCalculator::validVarName(string name, bool variableName, bool constName)
{
	bool validName = true;

	for (unsigned i = 0; i < ExpressionParser::RESERVED_NAMES_SIZE && validName; i++)
	{
		if (name == ExpressionParser::RESERVED_NAMES[i])
		{
			validName = false;
		}
	}

	if (validName)
	{
		if (this->baseParsingContext.functionExists(name)
			|| (!variableName && this->baseParsingContext.variableExists(name) && this->constantNames.count(name) == 0)
			|| (!constName && this->constantNames.count(name) > 0))
		{
			validName = false;
		}

		for (unsigned i = 0; i < name.size() && validName; i++)
		{
			if (!ExpressionParser::isVarChar(name[i]))
			{
				validName = false;
			}
		}
	}

	return validName;
}

bool ConsoleCalculator::validFunctionSignature(const FunctionSignature & functionSignature, bool savedFunction)
{
	bool validSignature = true;

	for (unsigned i = 0; i < ExpressionParser::RESERVED_NAMES_SIZE && validSignature; i++)
	{
		if (functionSignature.functionName == ExpressionParser::RESERVED_NAMES[i])
		{
			validSignature = false;
		}
	}

	if (validSignature)
	{
		if (this->baseParsingContext.variableExists(functionSignature.functionName)
			|| (savedFunction && this->baseParsingContext.functionExists(functionSignature) && this->savedFunctionSignatures.count(functionSignature) == 0)
			|| (!savedFunction && this->savedFunctionSignatures.count(functionSignature) > 0))
		{
			validSignature = false;
		}

		for (unsigned i = 0; i < functionSignature.functionName.size() && validSignature; i++)
		{
			if (!ExpressionParser::isVarChar(functionSignature.functionName[i]))
			{
				validSignature = false;
			}
		}
	}

	return validSignature;
}

void ConsoleCalculator::setConfiguration(wstring configFilePath)
{
	this->configFilePath = configFilePath;
	// string userString, angleString, outputModeString, fractionDecimalString, evaluationSettingsString, userConstantsString, savedFunctionsString;
	ifstream configFile(this->configFilePath);

	const wstring FILE_PARSE_FAILED_FORMAT = L"Could not parse the structure of configuration file \"%s\": %s, line %u",
		KEY_NOT_FOUND_FORMAT = L"Could not find key \"%s\" in configuration file \"%s\".",
		VERSION_NOT_FOUND_FORMAT = L"Could not find configuration version key \"%s\" in configuration file \"%s\"."
		L" The program will attempt to read the file with the current version of the configuration format (%s).",
		NEWER_VERSION_FORMAT = L"The configuration file \"%s\" is of a newer version (%s) than the latest that this program was compiled with (%s)."
		L" The program will attempt to read the file with the latter version.",
		KEY_PARSE_FAILED_FORMAT = L"The key \"%s\" in the configuration file \"%s\" could not be parsed.",
		CONSTANT_PARSE_FAILED_FORMAT = L"The string \"%s\" could not be parsed as a constant in the configuration file \"%s\".",
		CONSTANT_INVALID_NAME_FORMAT = L"The constant name \"%s\" is invalid in the configuration file \"%s\".";

	if (configFile.is_open())
	{
		string angleModeStr, decimalFractionStr, outputModeStr, precisionStr;
		bool fileParseSuccess = true;

		boost::property_tree::ptree configTree;

		try
		{
			boost::property_tree::read_xml(configFile, configTree);
		}
		catch (const boost::property_tree::xml_parser_error& ex)
		{
			fileParseSuccess = false;

			printUserMessage(ERROR_MESSAGE,
				boost::str(boost::wformat(FILE_PARSE_FAILED_FORMAT) % this->configFilePath % StringUtils::UTF8StringToWString(ex.message()) % ex.line()), wcout, wcerr);
		}

		configFile.close();

		if (fileParseSuccess)
		{
			int versionCompareResult = 0;
			string fileConfigVersion;

			try
			{
				fileConfigVersion = configTree.get<string>(CONFIG_VERSION_KEY);
				versionCompareResult = compareVersionStrings(fileConfigVersion, CONFIG_VERSION);
			}
			catch (const boost::property_tree::ptree_bad_path&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(VERSION_NOT_FOUND_FORMAT) % StringUtils::UTF8StringToWString(CONFIG_VERSION_KEY) % this->configFilePath % StringUtils::UTF8StringToWString(CONFIG_VERSION)), wcout, wcerr);
			}

			if (versionCompareResult == 1)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(NEWER_VERSION_FORMAT) % this->configFilePath % StringUtils::UTF8StringToWString(fileConfigVersion) % StringUtils::UTF8StringToWString(CONFIG_VERSION)), wcout, wcerr);
			}

			try
			{
				angleModeStr = configTree.get<string>(ANGLE_MODE_KEY);
				this->calculatorParser.parsingSettings.parseAngleMode = stringToEnum<ParsingSettings::angleMode>(angleModeStr, SHORT_ANGLE_STRING_TO_ANGLE_MODE);
			}
			catch (const boost::property_tree::ptree_bad_path&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_NOT_FOUND_FORMAT) % StringUtils::UTF8StringToWString(ANGLE_MODE_KEY) % this->configFilePath), wcout, wcerr);
			}
			catch (const EnumConversionException&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_PARSE_FAILED_FORMAT) % StringUtils::UTF8StringToWString(ANGLE_MODE_KEY) % this->configFilePath), wcout, wcerr);
			}

			try
			{
				decimalFractionStr = configTree.get<string>(DEC_FRAC_KEY);
				decimalFractionMode decFracMode = stringToEnum<decimalFractionMode>(decimalFractionStr, STRING_TO_DEC_FRAC_MODE);

				switch(decFracMode)
				{
				case decimalFractionMode::DECIMAL_VALUE:
					cout << CalcObj::decimalFormat;
					break;
				case decimalFractionMode::FRACTION_VALUE:
					cout << CalcObj::fractionFormat;
					break;
				}
			}
			catch (const boost::property_tree::ptree_bad_path&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_NOT_FOUND_FORMAT) % StringUtils::UTF8StringToWString(DEC_FRAC_KEY) % this->configFilePath), wcout, wcerr);
			}
			catch (const EnumConversionException&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_PARSE_FAILED_FORMAT) % StringUtils::UTF8StringToWString(DEC_FRAC_KEY) % this->configFilePath), wcout, wcerr);
			}

			try
			{
				outputModeStr = configTree.get<string>(OUTPUT_MODE_KEY);
				this->calcOutputMode = stringToEnum<outputMode>(outputModeStr, SHORT_STRING_TO_OUTPUT_MODE);
			}
			catch (const boost::property_tree::ptree_bad_path&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_NOT_FOUND_FORMAT) % StringUtils::UTF8StringToWString(OUTPUT_MODE_KEY) % this->configFilePath), wcout, wcerr);
			}
			catch (const EnumConversionException&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_PARSE_FAILED_FORMAT) % StringUtils::UTF8StringToWString(OUTPUT_MODE_KEY) % this->configFilePath), wcout, wcerr);
			}

			try
			{
				precisionStr = configTree.get<string>(PRECISION_MODE_KEY);
#ifdef MULTIPRECISION
				unsigned newPrecision;

				try
				{
					newPrecision = stringToUnsigned(precisionStr);

					if (newPrecision >= 1)
					{
						mpfr::mpreal::set_default_prec(mpfr::digits2bits(newPrecision + GUARD_DIGITS));
						outputPrecision = newPrecision;
					}
				}
				catch (const NumericConversionError&)
				{
					printUserMessage(WARNING_MESSAGE,
						boost::str(boost::wformat(KEY_PARSE_FAILED_FORMAT) % StringUtils::UTF8StringToWString(PRECISION_MODE_KEY) % this->configFilePath), wcout, wcerr);
				}
#else
				if (precisionStr == "max")
				{
					outputPrecision = numeric_limits<calcFloat>::digits10;
				}
				else
				{
					unsigned newPrecision;

					try
					{
						newPrecision = stringToUnsigned(precisionStr);

						if (newPrecision >= 1 && newPrecision <= numeric_limits<calcFloat>::digits10)
						{
							outputPrecision = newPrecision;
						}
					}
					catch (const NumericConversionError&)
					{
						printUserMessage(WARNING_MESSAGE,
							boost::str(boost::wformat(KEY_PARSE_FAILED_FORMAT) % StringUtils::UTF8StringToWString(PRECISION_MODE_KEY) % this->configFilePath), wcout, wcerr);
				}
			}
#endif
		}
			catch (const boost::property_tree::ptree_bad_path&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_NOT_FOUND_FORMAT) % StringUtils::UTF8StringToWString(PRECISION_MODE_KEY) % this->configFilePath), wcout, wcerr);
			}

			try
			{
				boost::property_tree::ptree constantTree = configTree.get_child(CONSTANTS_KEY);

				for (boost::property_tree::ptree::const_iterator thisIterator = constantTree.begin(); thisIterator != constantTree.end(); thisIterator++)
				{
					vector<string> userConstantPair;
					string constantString = thisIterator->second.data();
					StringUtils::split(constantString, ":", userConstantPair, 2);

					if (userConstantPair.size() != 2)
					{
						printUserMessage(WARNING_MESSAGE,
							boost::str(boost::wformat(CONSTANT_PARSE_FAILED_FORMAT) % StringUtils::UTF8StringToWString(constantString) % this->configFilePath), wcout, wcerr);
					}
					else if (!validVarName(userConstantPair[0], false, true))
					{
						printUserMessage(WARNING_MESSAGE,
							boost::str(boost::wformat(CONSTANT_INVALID_NAME_FORMAT) % StringUtils::UTF8StringToWString(userConstantPair[0]) % this->configFilePath), wcout, wcerr);
					}
					else
					{
						this->baseParsingContext.setVariable(userConstantPair[0], CalcObj(userConstantPair[1], true));
						this->constantNames.emplace(userConstantPair[0]);
					}
				}
			}
			catch (const boost::property_tree::ptree_bad_path&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_NOT_FOUND_FORMAT) % StringUtils::UTF8StringToWString(CONSTANTS_KEY) % this->configFilePath), wcout, wcerr);
			}

			try
			{
				boost::property_tree::ptree savedFunctionTree = configTree.get_child(SAVED_FUNCTIONS_KEY);

				for (boost::property_tree::ptree::const_iterator thisIterator = savedFunctionTree.begin(); thisIterator != savedFunctionTree.end(); thisIterator++)
				{
					pair<FunctionSignature, ExpressionParser::FunctionDefinition> functionDef = stringToFunction(thisIterator->second.data());
					this->baseParsingContext.setFunction(functionDef.first, functionDef.second);
					this->savedFunctionSignatures.emplace(functionDef.first);
				}
			}
			catch (const boost::property_tree::ptree_bad_path&)
			{
				printUserMessage(WARNING_MESSAGE,
					boost::str(boost::wformat(KEY_NOT_FOUND_FORMAT) % StringUtils::UTF8StringToWString(SAVED_FUNCTIONS_KEY) % this->configFilePath), wcout, wcerr);
			}
		}
	}
	else
	{
		printUserMessage(ERROR_MESSAGE,
						 boost::str(boost::wformat(CONFIG_FILE_OPEN_ERROR_FORMAT) % this->configFilePath), wcout, wcerr);
	}
}

void ConsoleCalculator::saveConfiguration()
{
	ofstream configFile;

	configFile.open(this->configFilePath.c_str(), ofstream::trunc);

	if(!configFile.fail())
	{
		boost::property_tree::ptree configTree;
		string angleModeStr;

		configTree.put(CONFIG_VERSION_KEY, CONFIG_VERSION);

		angleModeStr = enumToString<ParsingSettings::angleMode>(this->calculatorParser.parsingSettings.parseAngleMode, ANGLE_MODE_TO_SHORT_ANGLE_STRING);
		configTree.put(ANGLE_MODE_KEY, angleModeStr);

		configTree.put(OUTPUT_MODE_KEY, enumToString<outputMode>(this->calcOutputMode, OUTPUT_MODE_TO_SHORT_STRING));

		decimalFractionMode decFracMode = CalcObj::getStreamSettings(cout).decFracMode;
		configTree.put(DEC_FRAC_KEY, enumToString<decimalFractionMode>(decFracMode, DEC_FRAC_MODE_TO_STRING));

#ifdef MULTIPRECISION
		configTree.put(PRECISION_MODE_KEY, this->outputPrecision);
#else
		if(outputPrecision == numeric_limits<calcFloat>::digits10)
		{
			configTree.put(PRECISION_MODE_KEY, "max");
		}
		else
		{
			configTree.put(PRECISION_MODE_KEY, this->outputPrecision);
		}
#endif
		configTree.add(CONSTANTS_KEY, "");

		CalcObj::StreamSettings stringGenerationSettings;
		stringGenerationSettings.CSVOutputMode = false;
		stringGenerationSettings.decFracMode = DECIMAL_VALUE;
		stringGenerationSettings.fullPrecision = true;

		for(set<string>::const_iterator iter = constantNames.begin(); iter != constantNames.end(); iter++)
		{
			string constantString = (*iter) + ':' + (*(this->baseParsingContext.findVariable(*iter))).getString(stringGenerationSettings, 0);
			configTree.add(CONSTANT_KEY_PATH, constantString);
		}

		configTree.add(SAVED_FUNCTIONS_KEY, "");

		for(ExpressionParser::ParsingContext::FunctionIteratorConst iter = this->baseParsingContext.beginFunctionConst(); iter != this->baseParsingContext.endFunctionConst(); ++iter)
		{
			if(savedFunctionSignatures.count(iter.getFunctionSignature()) > 0)
			{
				configTree.add(SAVED_FUNCTION_KEY_PATH, functionToString(iter.getFunctionSignature(), *iter));
			}
		}

		boost::property_tree::write_xml(configFile, configTree);

		configFile.close();
	}
	else
	{
		printUserMessage(ERROR_MESSAGE,
			boost::str(boost::wformat(CONFIG_FILE_OPEN_ERROR_FORMAT) % this->configFilePath), wcout, wcerr);
	}
}

bool ConsoleCalculator::parseUserStringAux(string userString, bool csvFormat)
{
	vector<string> userStrings;
	StringUtils::split(userString, ";", userStrings);

		if(userStrings.size() > 1)
		{
			bool shouldTerminate = false;

			for(unsigned i = 0; i < userStrings.size() && !shouldTerminate; i++)
			{
				if (this->parseUserStringAux(userStrings[i], csvFormat))
				{
					shouldTerminate = true;
				}
			}

			return shouldTerminate;
		}
		else if (userString == "exit")
		{
			return true;
		}

		cout << setprecision(this->outputPrecision);

		if(userString == "")
		{
			if(inputHistory.size() > 0)
			{
				parseUserString(inputHistory[inputHistory.size() - 1], csvFormat);
				//cout << "Result: " << setprecision(numeric_limits<calcFloat>::digits10) << calcResult << endl;
				//resultHistory.push_back(calcResult);
			}
		}
		else
		{
			inputHistory.push_back(userString);

			vector<string> userStringParts;
			StringUtils::split(userString, " ", userStringParts);

			if(userStringParts.size() > 0 && userStringParts[0].length() > 0)
			{
				try
				{
					if(userStringParts[0] == "h" || userStringParts[0] == "help" /*tolower(userString[0]) == 'h' && userString.length() == 1*/)
					{
						// Open help file
						ifstream helpFileIn((this->helpFilePath).c_str());

						if(helpFileIn.fail())
						{
							//char fileNameBuf[FILENAME_MAX + 1];

							//wchar_t wtext[FILENAME_MAX + 1];
							//// mbstowcs(wtext, fileNameBuf, strlen(fileNameBuf)+1);// Plus null
							//LPWSTR ptr = wtext;
							//GetCurrentDirectory(FILENAME_MAX + 1, ptr);

							//wcstombs(fileNameBuf, ptr, FILENAME_MAX + 1);

							cout << "Error opening help file." << endl
								 << "Expected file path: ";

							wcout << helpFilePath;

							cout << endl;
						}
						else
						{
							while(!helpFileIn.eof())
							{
								string thisLine;
								getline(helpFileIn, thisLine, '\n');

								boost::replace_all(thisLine, "\t", "    "); // Replace tabs with 4 spaces to ensure that the file is displayed correctly
								cout << thisLine << endl;
							}
							helpFileIn.close();
						}
					}
					else if((userStringParts[0] == "t" || userStringParts[0] == "table") /*tolower(userStringParts[0][0]) == 'r' && userStringParts[0].length() == 1*/ && (userStringParts.size() == 5 || userStringParts.size() == 6))
					{
						//calcUnsignedInt numIter = calcUnsignedInt(userStringParts[1]);
						calcFloat iterStep, iterStart, iterEnd;
						string iterExpression = userStringParts[1],
							   iterVariable = userStringParts[2];

						if (!validVarName(iterVariable, false, false))
						{
							throw exception("Invalid iteration variable name used for table.");
						}

						ExpressionParser::ParsingContext iterContext(&(this->baseParsingContext));

						//if(numIter < 1)
						//{
						//	throw exception("Invalid number of iterations entered.");
						//}

						iterStart = this->calculatorParser.parseArithmetic(userStringParts[3], this->baseParsingContext).getVerifiedFloat("Type error in iteration start expression.");
						iterEnd = this->calculatorParser.parseArithmetic(userStringParts[4], this->baseParsingContext).getVerifiedFloat("Type error in iteration end expression.");

						// stringToFloat(userStringParts[3], iterStart);
						// stringToFloat(userStringParts[4], iterEnd);

						if(userStringParts.size() == 6)
						{
							iterStep = this->calculatorParser.parseArithmetic(userStringParts[5], this->baseParsingContext).getVerifiedFloat("Type error in iteration step expression.");
							// stringToFloat(userStringParts[5], iterStep);

							if(iterStep == 0.0)
							{
								throw exception("Invalid step entered (step cannot be zero).");
							}
							else if ((iterStep > 0 && iterStart > iterEnd)
								|| (iterStep < 0 && iterStart < iterEnd))
							{
								throw exception("Invalid step entered (sign of step is invalid).");
							}
						}
						else
						{
							iterStep = ((iterEnd - iterStart >= 0) ? 1.0 : -1.0);
						}

						//}
						//else
						//{
						//	runNStart = 1.0;
						//	runNStep = 1.0;
						//}
						unsigned columnWidth = outputPrecision + 6,
								 indentationWidth = 4,
								 indentedColumnWidth = columnWidth - indentationWidth;

						if(!csvFormat)
						{
							cout << left << setw(columnWidth) << ("Value of " + iterVariable) << " | " << "Result" << endl
								 << setfill('-') << setw(columnWidth) << '-' << "---" << setw(columnWidth) << '-' << setfill(' ') << endl;
						}
						else
						{
							cout << "BEGIN_TABLE" << endl;
						}

						unsigned numIterations = floatToUnsigned(floor(abs((iterEnd - iterStart) / iterStep))) + 1;

						for(unsigned thisIterationIndex = 0; thisIterationIndex < numIterations; thisIterationIndex++)
						{
							calcFloat iterVal = iterStart + (iterStep * thisIterationIndex);

							iterContext.setVariable(iterVariable, iterVal);

							CalcObj calcResult = this->calculatorParser.parseArithmetic(iterExpression, iterContext);

							if(!csvFormat)
							{
								cout << setw(columnWidth) << iterVal << " | ";

								CalcObj::StreamSettings outputSettings = CalcObj::getStreamSettings(cout);

								string valueStr = calcResult.getString(outputSettings, outputPrecision);

								if (valueStr.length() <= columnWidth)
								{
									cout << valueStr << endl;
								}
								else
								{
									cout.write(valueStr.data(), columnWidth);
									cout << endl;

									unsigned numWholeLines = (valueStr.length() - columnWidth) / indentedColumnWidth;

									for (unsigned i = 0; i < numWholeLines; i++)
									{
										cout << setw(columnWidth) << ' ' << " | " << setw(indentationWidth) << ' ';
										cout.write(valueStr.data() + (i * indentedColumnWidth + columnWidth), indentedColumnWidth);
										cout << endl;
									}

									long remainingCharacters = (valueStr.length() - columnWidth) % indentedColumnWidth;

									if (remainingCharacters > 0)
									{
										cout << setw(columnWidth) << ' ' << " | " << setw(indentationWidth) << ' ';
										cout.write(valueStr.data() + (valueStr.length() - remainingCharacters), remainingCharacters);
										cout << endl;
									}
								}
							}
							else
							{
								cout << iterVal << ',' << calcResult;

								if((iterStep > 0) ? (iterVal + iterStep > iterEnd) : (iterVal + iterStep < iterEnd))
								{
									cout << ',' << "END_TABLE";
								}

								cout << endl;
							}

							this->calculatorParser.resultHistory.push_back(calcResult);
						}
					}
					else if(userStringParts[0] == "c" || userStringParts[0] == "copy")
					{
						if(this->calculatorParser.resultHistory.size() > 0)
						{
							string cpyString = this->calculatorParser.resultHistory.back().getString(CalcObj::getStreamSettings(cout), outputPrecision);

							toClipboard(cpyString);
							cout << "Last result copied to clipboard." << endl;
						}
						else
						{
							cout << "No result exists to be copied." << endl;
						}
					}
					else if (userString == "deg")
					{
						this->calculatorParser.parsingSettings.parseAngleMode = ParsingSettings::degrees;
						cout << "Calculator angle mode set to degrees." << endl;
						this->saveConfiguration();
					}
					else if (userString == "rad")
					{
						this->calculatorParser.parsingSettings.parseAngleMode = ParsingSettings::radians;
						cout << "Calculator angle mode set to radians." << endl;
						this->saveConfiguration();
					}
					else if (userString == "grad")
					{
						this->calculatorParser.parsingSettings.parseAngleMode = ParsingSettings::gradians;
						cout << "Calculator angle mode set to gradians." << endl;
						this->saveConfiguration();
					}
					else if(userString == "auto" || userString == "scientific" || userString == "fixed")
					{
						if(userString == "auto")
						{
							calcOutputMode = outputMode::AUTO_MODE;
						}
						else if(userString == "scientific")
						{
							calcOutputMode = outputMode::SCIENTIFIC_MODE;
						}
						else if(userString == "fixed")
						{
							calcOutputMode = outputMode::FIXED_MODE;
						}

						cout << "Calculator output mode set to " << enumToString<outputMode>(calcOutputMode, OUTPUT_MODE_TO_LONG_STRING) << " notation." << endl;
						this->saveConfiguration();
					}
					else if((userStringParts[0] == "p" || userStringParts[0] == "setp") && userStringParts.size() == 2)
					{
						unsigned newPrecision;
						bool conversionSuccess = true;
						
						try
						{
							newPrecision = stringToUnsigned(userStringParts[1]);
						}
						catch (const NumericConversionError&)
						{
							conversionSuccess = false;
						}

#ifdef MULTIPRECISION
						if (conversionSuccess && newPrecision >= 1)
						{
							mpfr::mpreal::set_default_prec(mpfr::digits2bits(newPrecision + GUARD_DIGITS));
							outputPrecision = newPrecision;
							cout << "Precision set to " << outputPrecision << ' ' << ((outputPrecision != 1) ? "digits" : "digit") << '.' << endl;
							this->saveConfiguration();
						}
						else
						{
							throw exception("Invalid precision specified.");
						}
#else
						if(userStringParts[1] == "max")
						{
							outputPrecision = numeric_limits<calcFloat>::digits10;
							cout << "Precision set to " << outputPrecision << ' ' << ((outputPrecision != 1) ? "digits" : "digit") << '.' << endl;
							this->saveConfiguration();
						}
						else
						{
							if(conversionSuccess && newPrecision >= 1 && newPrecision <= numeric_limits<calcFloat>::digits10)
							{
								outputPrecision = newPrecision;
								cout << "Precision set to " << outputPrecision << ' ' << ((outputPrecision != 1) ? "digits" : "digit") << '.' << endl;
								this->saveConfiguration();
							}
							else
							{
								throw exception("Invalid precision specified.");
							}
						}
#endif
					}
					else if((userStringParts[0] == "fn" || userStringParts[0] == "setfn") && (userStringParts.size() == 2 || userStringParts.size() == 3))
					{
						pair<FunctionSignature, ExpressionParser::FunctionDefinition> newFunction = stringToFunction(userStringParts[1]);
						bool savedFunction = (userStringParts.size() == 3 && userStringParts[2] == "-save");

						if(!validFunctionSignature(newFunction.first, savedFunction))
						{
							cout << "Invalid function name." << endl;
						}
						else
						{
							this->baseParsingContext.setFunction(newFunction.first, newFunction.second);

							if (savedFunction)
							{
								savedFunctionSignatures.emplace(newFunction.first);
								this->saveConfiguration();
							}

							cout << "Function successfully created." << endl;
						}
					}
					else if((userStringParts[0] == "v" || userStringParts[0] == "setvar") && userStringParts.size() == 3)
					{
						if(validVarName(userStringParts[1], true, false))
						{
							CalcObj varValue = this->calculatorParser.parseArithmetic(userStringParts[2], this->baseParsingContext);

							this->baseParsingContext.setVariable(userStringParts[1], varValue);

							setUserFlags(cout, calcOutputMode);

							cout << "Set variable \"" << userStringParts[1] << "\" to " << varValue << '.' << endl;
						}
						else
						{
							cout << "Invalid variable name." << endl;
						}
					}
					else if((userStringParts[0] == "v" || userStringParts[0] == "setvar") && userStringParts.size() == 4 && userStringParts[3] == "-const")
					{
						if(validVarName(userStringParts[1], false, true))
						{
							CalcObj constValue = this->calculatorParser.parseArithmetic(userStringParts[2], this->baseParsingContext);

							setUserFlags(cout, calcOutputMode);

							this->baseParsingContext.setVariable(userStringParts[1], constValue);
							this->constantNames.emplace(userStringParts[1]);
							cout << "Set constant \"" << userStringParts[1] << "\" to " << constValue << '.' << endl;
							this->saveConfiguration();
						}
						else
						{
							cout << "Invalid constant name." << endl;
						}
					}
					else if((userStringParts[0] == "d" || userStringParts[0] == "delvar") && userStringParts.size() == 2)
					{
						if(this->baseParsingContext.variableExists(userStringParts[1]) && constantNames.count(userStringParts[1]) == 0)
						{
							this->baseParsingContext.deleteVariableFromCurrent(userStringParts[1]);
							cout << "Variable \"" << userStringParts[1] << "\" deleted." << endl;
						}
						else if(userStringParts[1] == "-all")
						{
							for (ExpressionParser::ParsingContext::VariableIteratorConst iter = this->baseParsingContext.beginVariableConst(); iter != this->baseParsingContext.endVariableConst(); ++iter)
							{
								if (constantNames.count(iter.getVariableName()) == 0)
								{
									this->baseParsingContext.deleteVariableFromCurrent(iter.getVariableName());
								}
							}
							cout << "All variables deleted." << endl;
						}
						else
						{
							cout << "Variable name not found." << endl;
						}
					}
					else if((userStringParts[0] == "d" || userStringParts[0] == "delvar") && userStringParts.size() == 3 && userStringParts[2] == "-const")
					{
						if(constantNames.count(userStringParts[1]) > 0)
						{
							this->baseParsingContext.deleteVariableFromCurrent(userStringParts[1]);
							constantNames.erase(userStringParts[1]);

							cout << "Constant \"" << userStringParts[1] << "\" deleted." << endl;
							this->saveConfiguration();
						}
						else if(userStringParts[1] == "-all")
						{
							for (ExpressionParser::ParsingContext::VariableIteratorConst iter = this->baseParsingContext.beginVariableConst(); iter != this->baseParsingContext.endVariableConst(); ++iter)
							{
								if (constantNames.count(iter.getVariableName()) > 0)
								{
									this->baseParsingContext.deleteVariableFromCurrent(iter.getVariableName());
									this->constantNames.erase(iter.getVariableName());
								}
							}

							cout << "All constants deleted." << endl;
							this->saveConfiguration();
						}
						else
						{
							cout << "Constant name not found." << endl;
						}
					}
					else if((userStringParts[0] == "df" || userStringParts[0] == "deletefunction") && userStringParts.size() == 3)
					{
						unsigned numParams = stoul(userStringParts[2]);
						FunctionSignature fnSignature = { userStringParts[1], numParams };

						ExpressionParser::ParsingContext::FunctionIteratorConst currentFunction = this->baseParsingContext.findFunction(fnSignature);

						if(currentFunction != this->baseParsingContext.endFunctionConst())
						{
							this->baseParsingContext.deleteFunctionFromCurrent(fnSignature);

							if(this->savedFunctionSignatures.count(fnSignature) > 0)
							{
								this->savedFunctionSignatures.erase(fnSignature);
								this->saveConfiguration();
							}

							cout << "Function successfully deleted." << endl;
						}
						else
						{
							cout << "The function specified was not found." << endl;
						}
					}
					else if((userStringParts[0] == "l" || userStringParts[0] == "listvars") && userStringParts.size() == 1)
					{
						if(this->constantNames.size() < this->baseParsingContext.getNumVariables())
						{
							cout << CalcObj::setFullPrecision;

							for (ExpressionParser::ParsingContext::VariableIteratorConst iter = this->baseParsingContext.beginVariableConst(); iter != this->baseParsingContext.endVariableConst(); ++iter)
							{
								if (constantNames.count(iter.getVariableName()) == 0)
								{
									cout << iter.getVariableName() << ": " << (*iter) << endl;
								}
							}

							cout << CalcObj::unsetFullPrecision;
						}
						else
						{
							cout << "No variables stored." << endl;
						}
					}
					else if((userStringParts[0] == "l" || userStringParts[0] == "listvars") && userStringParts.size() == 2 && userStringParts[1] == "-const")
					{
						if(this->constantNames.size() > 0)
						{
							cout << CalcObj::setFullPrecision;

							for (ExpressionParser::ParsingContext::VariableIteratorConst iter = this->baseParsingContext.beginVariableConst(); iter != this->baseParsingContext.endVariableConst(); ++iter)
							{
								if (constantNames.count(iter.getVariableName()) > 0)
								{
									cout << iter.getVariableName() << ": " << (*iter) << endl;
								}
							}

							cout << CalcObj::unsetFullPrecision;
						}
						else
						{
							cout << "No constants stored." << endl;
						}
					}
					else if((userStringParts[0] == "lf" || userStringParts[0] == "listfunctions") && userStringParts.size() == 1)
					{
						if(this->baseParsingContext.getNumFunctions() > 0)
						{
							// cout << setprecision(numeric_limits<calcFloat>::digits10);

							for(ExpressionParser::ParsingContext::FunctionIteratorConst iter = this->baseParsingContext.beginFunctionConst(); iter != this->baseParsingContext.endFunctionConst(); ++iter)
							{
								cout << functionToString(iter.getFunctionSignature(), *iter);

								if(this->savedFunctionSignatures.count(iter.getFunctionSignature()) > 0)
								{
									cout << " (saved)";
								}

								cout << endl;
							}
						}
						else
						{
							cout << "No functions stored." << endl;
						}
					}
					else if(userStringParts[0] == "frac")
					{
						cout << CalcObj::fractionFormat;
						saveConfiguration();

						cout << "Calculator set to fraction output mode." << endl;
					}
					else if(userStringParts[0] == "dec")
					{
						cout << CalcObj::decimalFormat;
						saveConfiguration();

						cout << "Calculator set to decimal output mode." << endl;
					}
					else
					{
						// cout << "Parser setting test: " << static_cast<int>(this->calculatorParser.parsingSettings.enableIntegralFn) << endl;

						CalcObj calcResult = this->calculatorParser.parseArithmetic(userString, this->baseParsingContext);

						setUserFlags(cout, calcOutputMode);

						if(!csvFormat)
						{
							cout << "Result: " << calcResult << endl;
						}
						else
						{
							cout << calcResult << endl;
						}

						this->calculatorParser.resultHistory.push_back(calcResult);
					}
				}
				catch(const exception& e)
				{
					cout << e.what() << endl;
				}
			}
		}

		return false;
}

void ConsoleCalculator::parseUserString(string userString, bool csvFormat)
{
	this->parseUserStringAux(userString, csvFormat);
}

void ConsoleCalculator::runPromptLoop()
{
	string angleString,
		   fractionDecimalString,
		   userString;

	bool shouldTerminate = false;

	do
	{
		angleString = enumToString<ParsingSettings::angleMode>(this->calculatorParser.parsingSettings.parseAngleMode, ANGLE_MODE_TO_SHORT_ANGLE_STRING);

		decimalFractionMode currentDecimalFractionMode = CalcObj::getStreamSettings(cout).decFracMode;
		fractionDecimalString = enumToString<decimalFractionMode>(currentDecimalFractionMode, DEC_FRAC_MODE_TO_STRING);

		cout << '[' << angleString << ' ' << enumToString<outputMode>(calcOutputMode, OUTPUT_MODE_TO_SHORT_STRING) << ' ' << fractionDecimalString << ' ' << outputPrecision << ']' << " >> ";
		std::getline(cin, userString);

		if(this->parseUserStringAux(userString))
		{
			shouldTerminate = true;
		}
	}
	while (!shouldTerminate);
}