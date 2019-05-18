#include <string>
#include <map>
#include <vector>
#include <algorithm>

// #include <iostream>

#include "parseArithmetic.h"

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;
#ifdef MULTIPRECISION
using namespace boost::multiprecision;
#endif

const string ExpressionParser::RESERVED_NAMES[] = { "e", "pi" };
const unsigned ExpressionParser::RESERVED_NAMES_SIZE = 2;

const string functionNamesArr[] = {"sin", "cos", "tan", "arcsin", "arccos", "arctan", "sqrt", "ln", "log", "logbase", "ans", "deriv", "integral", "solve", "abs", "ceil", "floor", "round", "primeFct", "isPrime", "gcd", "lcm", "rand", "avg",
								   "variance", "max", "min", "stddev", "sum", "product", "seq", "foreach", "filter", "getIndex", "setIndex", "length", "concat", "sublist", "or", "and",
								   "not", "if"};
const size_t FUNCTION_NAMES_LENGTH = 42;

const string additionalImplicitMultLeftStrs[] = { ")", "}" };
const size_t ADDITIONAL_IMPLICIT_MULT_LEFT_STRS_LENGTH = 2;

const string additionalImplicitMultRightStrs[] = { "(", "{" };
const size_t ADDITIONAL_IMPLICIT_MULT_RIGHT_STRS_LENGTH = 2;

/*
vector<calcFloat> gaussNodes = {0.0514718425553176, 0.1538699136085830, 0.2546369261678890, 0.3527047255308780, 0.4470337695380890, 0.5366241481420190, 0.6205261829892420,
							 0.6978504947933150, 0.7677774321048260, 0.8295657623827680, 0.8825605357920520, 0.9262000474292740, 0.9600218649683070, 0.9836681232797470,
							 0.9968934840746490, -0.0514718425553176, -0.1538699136085830, -0.2546369261678890, -0.3527047255308780, -0.4470337695380890, -0.5366241481420190,
							 -0.6205261829892420, -0.6978504947933150, -0.7677774321048260, -0.8295657623827680, -0.8825605357920520, -0.9262000474292740, -0.9600218649683070,
							 -0.9836681232797470, -0.9968934840746490}, 

		  gaussWeights; {0.1028526528935580, 0.1017623897484050, 0.0995934205867952, 0.0963687371746442, 0.0921225222377861, 0.0868997872010829, 0.0807558952294202,
							   0.0737559747377052, 0.0659742298821804, 0.0574931562176190, 0.0484026728305940, 0.0387991925696270, 0.0287847078833233, 0.0184664683110909,
							   0.0079681924961666, 0.1028526528935580, 0.1017623897484050, 0.0995934205867952, 0.0963687371746442, 0.0921225222377861, 0.0868997872010829,
							   0.0807558952294202, 0.0737559747377052, 0.0659742298821804, 0.0574931562176190, 0.0484026728305940, 0.0387991925696270, 0.0287847078833233,
							   0.0184664683110909, 0.0079681924961666}; */

#ifndef MULTIPRECISION
const vector<calcFloat> gaussNodes = { -0.99886640442007105, -0.994031969432090713, -0.985354084048005882, -0.972864385106692074, -0.956610955242807943, -0.936656618944877934, -0.913078556655791893, -0.885967979523613049
, -0.855429769429946085, -0.82158207085933595, -0.78455583290039926, -0.744494302226068538, -0.701552468706822251, -0.655896465685439361, -0.607702927184950239, -0.557158304514650054
, -0.504458144907464202, -0.449806334974038789, -0.393414311897565127, -0.335500245419437357, -0.27628819377953199, -0.216007236876041757, -0.154890589998145902, -0.093174701560086141
, -0.031098338327188876, 0.031098338327188876, 0.093174701560086141, 0.154890589998145902, 0.216007236876041757, 0.27628819377953199, 0.335500245419437357, 0.393414311897565127
, 0.449806334974038789, 0.504458144907464202, 0.557158304514650054, 0.607702927184950239, 0.655896465685439361, 0.701552468706822251, 0.744494302226068538, 0.784555832900399264
, 0.821582070859335948, 0.855429769429946085, 0.885967979523613049, 0.913078556655791893, 0.936656618944877934, 0.956610955242807943, 0.972864385106692074, 0.985354084048005882
, 0.994031969432090713, 0.99886640442007105 };

const vector<calcFloat> gaussWeights = { 0.002908622553155141, 0.006759799195745402, 0.010590548383650969, 0.0143808227614855744, 0.01811556071348939, 0.021780243170124793, 0.02536067357001239, 0.028842993580535198
, 0.032213728223578017, 0.03545983561514615, 0.03856875661258768, 0.041528463090147697, 0.044327504338803275, 0.046955051303948433, 0.04940093844946631, 0.05165570306958114
, 0.053710621888996247, 0.0555577448062125176, 0.057189925647728384, 0.058600849813222446, 0.059785058704265458, 0.060737970841770216, 0.06145589959031666, 0.061936067420683243
, 0.06217661665534726, 0.062176616655347262, 0.061936067420683243, 0.06145589959031666, 0.060737970841770216, 0.05978505870426546, 0.05860084981322245, 0.057189925647728384
, 0.05555774480621252, 0.05371062188899625, 0.051655703069581138, 0.04940093844946631, 0.046955051303948433, 0.044327504338803275, 0.041528463090147697, 0.038568756612587675
, 0.03545983561514615, 0.032213728223578017, 0.028842993580535198, 0.02536067357001239, 0.021780243170124793, 0.01811556071348939, 0.014380822761485574, 0.010590548383650969
, 0.006759799195745402, 0.002908622553155141 };
#endif


/*
vector<calcFloat> doublePrecisisonGaussNodes = { -0.9997137267734412336782, -0.9984919506395958184002, -0.996295134733125149186, -0.993124937037443459652, -0.9889843952429917480044
												 -0.983877540706057015496, -0.977809358486918288554, -0.970785775763706331931, -0.962813654255815527294, -0.9539007829254917428493
												 
-0.944055870136255977963
-0.933288535043079545924
-0.921609298145333952667
-0.9090295709825296904671
-0.8955616449707269866985
-0.8812186793850184155733
-0.8660146884971646234107
-0.8499645278795912842934
-0.8330838798884008235429
-0.815389238339176254394
-0.79689789239031447639
-0.7776279096494954756276
-0.7575981185197071760357
-0.7368280898020207055124
-0.71533811757305644646
-0.6931491993558019659487
-0.670283015603141015803
-0.6467619085141292798326
-0.622608860203707771604
-0.5978474702471787212648
-0.5725019326213811913169
-0.546597012065094167468
-0.520158019881763056647
-0.493210789208190933569
-0.4657816497733580422492
-0.437897402172031513109
-0.4095852916783015425289
-0.3808729816246299567634
-0.3517885263724217209723
-0.3223603439005291517225
-0.2926171880384719647376
-0.2625881203715034791689
-0.2323024818449739696495
-0.2017898640957359972361
-0.1710800805386032748875
-0.140203137236113973208
-0.109189203580061115003
-0.078068582813436636695
-0.046871682421591631615
-0.0156289844215430828722
0.0156289844215430828722
0.04687168242159163161492
0.07806858281343663669482
0.1091892035800611150034
0.1402031372361139732075
0.171080080538603274888
0.2017898640957359972361
0.23230248184497396965
0.2625881203715034791689
0.2926171880384719647376
0.3223603439005291517225
0.351788526372421720972
0.3808729816246299567634
0.409585291678301542529
0.437897402172031513109
0.465781649773358042249
0.4932107892081909335693
0.5201580198817630566468
0.546597012065094167468
0.5725019326213811913169
0.597847470247178721265
0.6226088602037077716042
0.6467619085141292798326
0.6702830156031410158026
0.6931491993558019659487
0.71533811757305644646
0.736828089802020705512
0.7575981185197071760357
0.777627909649495475628
0.7968978923903144763896
0.815389238339176254394
0.833083879888400823543
0.8499645278795912842934
0.8660146884971646234107
0.8812186793850184155733
0.895561644970726986699
0.9090295709825296904671
0.921609298145333952667
0.9332885350430795459243
0.9440558701362559779628
0.953900782925491742849
0.9628136542558155272937
0.970785775763706331931
0.977809358486918288554
0.9838775407060570154961
0.988984395242991748004
0.993124937037443459652
0.9962951347331251491861
0.9984919506395958184002
0.999713726773441233678 7};
*/

// const size_t GAUSS_NODES_LENGTH = 30;

const vector<string> functionNames = vector<string>(functionNamesArr, functionNamesArr + FUNCTION_NAMES_LENGTH);

bool ExpressionParser::FunctionSignature::operator<(const FunctionSignature & otherSignature) const
{
	return (this->functionName < otherSignature.functionName)
		|| (this->functionName == otherSignature.functionName && this->numParams < otherSignature.numParams);
}

bool ExpressionParser::FunctionSignature::operator<=(const FunctionSignature & otherSignature) const
{
	return (*this < otherSignature || *this == otherSignature);
}

bool ExpressionParser::FunctionSignature::operator>(const FunctionSignature & otherSignature) const
{
	return !(*this <= otherSignature);
}

bool ExpressionParser::FunctionSignature::operator>=(const FunctionSignature & otherSignature) const
{
	return !(*this < otherSignature);
}

bool ExpressionParser::FunctionSignature::operator==(const FunctionSignature & otherSignature) const
{
	return (this->functionName == otherSignature.functionName)
		&& (this->numParams == otherSignature.numParams);
}

bool ExpressionParser::FunctionSignature::operator!=(const FunctionSignature & otherSignature) const
{
	return !(*this == otherSignature);
}

ExpressionParser::ParsingContext::VariableIteratorConst::VariableIteratorConst(bool isEndIterator, const ParsingContext* parsingContext, const ParsingContext* baseContext, std::map<std::string, calcObj>::const_iterator variableMapIterator):
	isEndIterator(isEndIterator),
	parsingContext(parsingContext),
	baseContext(baseContext),
	constMapIterator(variableMapIterator)
{

}

string ExpressionParser::ParsingContext::VariableIteratorConst::getVariableName() const
{
	return this->constMapIterator->first;
}

const calcObj& ExpressionParser::ParsingContext::VariableIteratorConst::operator*() const
{
	return this->constMapIterator->second;
}

ExpressionParser::ParsingContext::VariableIteratorConst& ExpressionParser::ParsingContext::VariableIteratorConst::operator++()
{
	if (!this->isEndIterator)
	{
		this->constMapIterator++;
		if (this->constMapIterator == this->parsingContext->valueMap.end())
		{
			do
			{
				this->parsingContext = this->parsingContext->inheritedContext;
			} while (this->parsingContext != NULL && this->parsingContext->valueMap.empty());

			if (this->parsingContext != NULL)
			{
				this->constMapIterator = this->parsingContext->valueMap.begin();
			}
			else
			{
				this->isEndIterator = true;
			}
		}
	}

	return (*this);
}

bool ExpressionParser::ParsingContext::VariableIteratorConst::operator==(const VariableIteratorConst & otherIterator)
{
	bool resultValue = false;

	if (this->baseContext == otherIterator.baseContext)
	{
		if (this->isEndIterator && otherIterator.isEndIterator)
		{
			resultValue = true;
		}
		else if ((!this->isEndIterator && !otherIterator.isEndIterator)
			&& (this->parsingContext == otherIterator.parsingContext)
			&& (this->constMapIterator == otherIterator.constMapIterator))
		{
			resultValue = true;
		}
	}

	return resultValue;
}

bool ExpressionParser::ParsingContext::VariableIteratorConst::operator!=(const VariableIteratorConst & otherIterator)
{
	return !(*this == otherIterator);
}

ExpressionParser::ParsingContext::FunctionIteratorConst::FunctionIteratorConst(bool isEndIterator, const ParsingContext * parsingContext, const ParsingContext * baseContext, std::map<FunctionSignature, FunctionDefinition>::const_iterator functionMapIterator):
	isEndIterator(isEndIterator),
	parsingContext(parsingContext),
	baseContext(baseContext),
	constMapIterator(functionMapIterator)
{
}

ExpressionParser::FunctionSignature ExpressionParser::ParsingContext::FunctionIteratorConst::getFunctionSignature()
{
	return (this->constMapIterator->first);
}

const ExpressionParser::FunctionDefinition& ExpressionParser::ParsingContext::FunctionIteratorConst::operator*()
{
	return (this->constMapIterator->second);
}

ExpressionParser::ParsingContext::FunctionIteratorConst& ExpressionParser::ParsingContext::FunctionIteratorConst::operator++()
{
	if (!this->isEndIterator)
	{
		this->constMapIterator++;
		if (this->constMapIterator == this->parsingContext->functionMap.end())
		{
			do
			{
				this->parsingContext = this->parsingContext->inheritedContext;
			} while (this->parsingContext != NULL && this->parsingContext->functionMap.empty());

			if (this->parsingContext != NULL)
			{
				this->constMapIterator = this->parsingContext->functionMap.begin();
			}
			else
			{
				this->isEndIterator = true;
			}
		}
	}

	return (*this);
}

bool ExpressionParser::ParsingContext::FunctionIteratorConst::operator==(const FunctionIteratorConst & otherIterator)
{
	bool resultValue = false;

	if (this->baseContext == otherIterator.baseContext)
	{
		if (this->isEndIterator && otherIterator.isEndIterator)
		{
			resultValue = true;
		}
		else if ((!this->isEndIterator && !otherIterator.isEndIterator)
			&& (this->parsingContext == otherIterator.parsingContext)
			&& (this->constMapIterator == otherIterator.constMapIterator))
		{
			resultValue = true;
		}
	}

	return resultValue;
}

bool ExpressionParser::ParsingContext::FunctionIteratorConst::operator!=(const FunctionIteratorConst & otherIterator)
{
	return !(*this == otherIterator);
}

ExpressionParser::ParsingContext::ParsingContext()
{

}

ExpressionParser::ParsingContext::ParsingContext(const std::map<std::string, calcObj>& valueMap, const std::map<FunctionSignature, FunctionDefinition>& functionMap, const std::vector<calcObj>& resultHistory):
	valueMap(valueMap),
	functionMap(functionMap),
	resultHistory(resultHistory)
{

}

ExpressionParser::ParsingContext::ParsingContext(const ParsingContext * inheritedContext):
	inheritedContext(inheritedContext)
{

}

ExpressionParser::ParsingContext::VariableIteratorConst ExpressionParser::ParsingContext::beginVariableConst() const
{
	const ParsingContext* firstNonEmptyContext = this;

	while (firstNonEmptyContext != NULL && firstNonEmptyContext->valueMap.empty())
	{
		firstNonEmptyContext = firstNonEmptyContext->inheritedContext;
	}

	if (firstNonEmptyContext != NULL)
	{
		return VariableIteratorConst(false, firstNonEmptyContext, this, firstNonEmptyContext->valueMap.begin());
	}
	else
	{
		return this->endVariableConst();
	}
}

ExpressionParser::ParsingContext::VariableIteratorConst ExpressionParser::ParsingContext::endVariableConst() const
{
	return VariableIteratorConst(true, NULL, this, this->valueMap.begin());
}

ExpressionParser::ParsingContext::FunctionIteratorConst ExpressionParser::ParsingContext::beginFunctionConst() const
{
	const ParsingContext* firstNonEmptyContext = this;

	while (firstNonEmptyContext != NULL && firstNonEmptyContext->functionMap.empty())
	{
		firstNonEmptyContext = firstNonEmptyContext->inheritedContext;
	}

	if (firstNonEmptyContext != NULL)
	{
		return FunctionIteratorConst(false, firstNonEmptyContext, this, firstNonEmptyContext->functionMap.begin());
	}
	else
	{
		return this->endFunctionConst();
	}
}

ExpressionParser::ParsingContext::FunctionIteratorConst ExpressionParser::ParsingContext::endFunctionConst() const
{
	return FunctionIteratorConst(true, NULL, this, this->functionMap.begin());
}

void ExpressionParser::ParsingContext::setVariable(const string & newVariableName, const calcObj & newVariable)
{
	valueMap[newVariableName] = newVariable;
}

void ExpressionParser::ParsingContext::setFunction(const FunctionSignature & newFunctionSignature, const FunctionDefinition & newFunction)
{
	if (this->functionMap.count(newFunctionSignature) == 0)
	{
		this->functionNameMap.insert(pair<string, FunctionSignature>(newFunctionSignature.functionName, newFunctionSignature));
	}

	this->functionMap[newFunctionSignature] = newFunction;
}

bool ExpressionParser::ParsingContext::variableExists(const string & variableName) const
{
	const ParsingContext* currentContext = this;
	bool foundVariable = false;

	do
	{
		if (currentContext->valueMap.count(variableName) > 0)
		{
			foundVariable = true;
		}
		else
		{
			currentContext = currentContext->inheritedContext;
		}
	} while (currentContext != NULL && !foundVariable);

	return foundVariable;
}

bool ExpressionParser::ParsingContext::functionExists(const string & functionName) const
{
	const ParsingContext* currentContext = this;
	bool foundFunction = false;

	do
	{
		if (currentContext->functionNameMap.count(functionName) > 0)
		{
			foundFunction = true;
		}
		else
		{
			currentContext = currentContext->inheritedContext;
		}
	} while (currentContext != NULL && !foundFunction);

	return foundFunction;
}

bool ExpressionParser::ParsingContext::functionExists(const FunctionSignature & functionSignature) const
{
	const ParsingContext* currentContext = this;
	bool foundFunction = false;

	do
	{
		if (currentContext->functionMap.count(functionSignature) > 0)
		{
			foundFunction = true;
		}
		else
		{
			currentContext = currentContext->inheritedContext;
		}
	} while (currentContext != NULL && !foundFunction);

	return foundFunction;
}

ExpressionParser::ParsingContext::VariableIteratorConst ExpressionParser::ParsingContext::findVariable(const string & variableName) const
{
	map<string, calcObj>::const_iterator varIterator;
	const ParsingContext* currentContext = this;
	bool varFound = false;

	do
	{
		varIterator = currentContext->valueMap.find(variableName);

		if (varIterator == currentContext->valueMap.cend())
		{
			currentContext = currentContext->inheritedContext;
		}
		else
		{
			varFound = true;
		}

	} while (currentContext != NULL && !varFound);

	if (varFound)
	{
		return VariableIteratorConst(false, currentContext, this, varIterator);
	}
	else
	{
		return this->endVariableConst();
	}
}

ExpressionParser::ParsingContext::FunctionIteratorConst ExpressionParser::ParsingContext::findFunction(const FunctionSignature& functionSignature) const
{
	map<FunctionSignature, FunctionDefinition>::const_iterator functionIterator;
	const ParsingContext* currentContext = this;
	bool functionFound = false;

	do
	{
		functionIterator = currentContext->functionMap.find(functionSignature);

		if (functionIterator == currentContext->functionMap.cend())
		{
			currentContext = currentContext->inheritedContext;
		}
		else
		{
			functionFound = true;
		}

	} while (currentContext != NULL && !functionFound);

	if (functionFound)
	{
		return FunctionIteratorConst(false, currentContext, this, functionIterator);
	}
	else
	{
		return this->endFunctionConst();
	}
}

unsigned ExpressionParser::ParsingContext::getNumVariables() const
{
	return this->valueMap.size();
}

unsigned ExpressionParser::ParsingContext::getNumFunctions() const
{
	return this->functionMap.size();
}

bool ExpressionParser::ParsingContext::deleteVariableFromCurrent(const string& variableName)
{
	unsigned elementsRemoved = this->valueMap.erase(variableName);

	return (elementsRemoved >= 1);
}

bool ExpressionParser::ParsingContext::deleteFunctionFromCurrent(const FunctionSignature & functionSignature)
{
	unsigned elementsRemoved = this->functionMap.erase(functionSignature);
	bool resultValue = false;

	if (elementsRemoved >= 1)
	{
		pair<multimap<string, FunctionSignature>::iterator,
			 multimap<string, FunctionSignature>::iterator> functionNameRange = this->functionNameMap.equal_range(functionSignature.functionName);
		bool functionFound = false;
		multimap<string, FunctionSignature>::iterator functionElement = functionNameMap.end();

		for (multimap<string, FunctionSignature>::iterator thisElement = functionNameRange.first;
			thisElement != functionNameRange.second && !functionFound; thisElement++)
		{
			if (thisElement->second.numParams == functionSignature.numParams)
			{
				functionElement = thisElement;
				functionFound = true;
			}
		}

		if (functionFound)
		{
			resultValue = true;
			this->functionNameMap.erase(functionElement);
		}
	}

	return resultValue;
}

bool ExpressionParser::parseListString(const std::string& listStr, calcObj& parsedObj, const ParsingContext& parsingContext) const
{
	int parenNestLevel = 0, braceNestLevel = 0;
	string thisItem = "";
	vector<calcFloat> parsedValues;
	bool returnVal = true;

	if(listStr.length() >= 2 && listStr[0] == '{' && findClosingChar('{', '}', 0, listStr) == listStr.length() - 1)
	{
		string trimmedStr = listStr.substr(1, listStr.length() - 2);

		for(unsigned i = 0; i < trimmedStr.size(); i++)
		{
			if(trimmedStr[i] == '(')
			{
				parenNestLevel++;
			}
			else if(trimmedStr[i] == ')')
			{
				parenNestLevel--;
			}

			if(trimmedStr[i] == '{')
			{
				braceNestLevel++;
			}
			else if(trimmedStr[i] == '}')
			{
				braceNestLevel--;
			}

			if(parenNestLevel == 0 && braceNestLevel == 0)
			{
				if(trimmedStr[i] == ',')
				{
					calcObj thisObj = parseArithmetic(thisItem, parsingContext, listStr);

					parsedValues.push_back(thisObj.getVerifiedFloat("List literal type error"));

					thisItem = "";
				}
				else
				{
					thisItem += trimmedStr[i];
				}
			}
			else
			{
				thisItem += trimmedStr[i];
			}
		}

		if(thisItem.length() > 0)
		{
			calcObj thisObj = parseArithmetic(thisItem, parsingContext, listStr);

			parsedValues.push_back(thisObj.getVerifiedFloat("List literal type error"));
		}

		parsedObj.set_list(parsedValues);
	}
	else
	{
		returnVal = false;
	}

	return returnVal;
}

bool ExpressionParser::isValidImplicitTerm(string expression, unsigned basePosition, const ParsingContext& parsingContext) const
{
	bool leftFunctionName = false,
		validLeftStr = false,
		validRightStr = false,
		validImplicitMultExpression = false;

	if (isVarChar(expression[basePosition]) || isdigit(expression[basePosition]))
	{
		if (!(expression[basePosition] == 'E' && !(isVarChar(expression[basePosition + 1])) && (basePosition < 1 || !isVarChar(expression[basePosition - 1]))))
		{
			validLeftStr = true;
		}
	}
	else
	{
		for (unsigned leftImplicitMultTermIndex = 0; leftImplicitMultTermIndex < ADDITIONAL_IMPLICIT_MULT_LEFT_STRS_LENGTH && !validLeftStr; leftImplicitMultTermIndex++)
		{
			int leftSectionStartIndex = (int)basePosition - (additionalImplicitMultLeftStrs[leftImplicitMultTermIndex].length() - 1);

			if (leftSectionStartIndex >= 0)
			{
				string expressionSection = expression.substr(leftSectionStartIndex, additionalImplicitMultLeftStrs[leftImplicitMultTermIndex].length());

				if (expressionSection == additionalImplicitMultLeftStrs[leftImplicitMultTermIndex])
				{
					validLeftStr = true;
				}
			}
		}
	}

	if (isVarChar(expression[basePosition + 1]) || isdigit(expression[basePosition + 1]))
	{
		if (!(expression[basePosition + 1] == 'E' && !isVarChar(expression[basePosition]) && (basePosition > (expression.size() - 3) || !isVarChar(expression[basePosition + 2]))))
		{
			validRightStr = true;
		}
	}
	else
	{
		for (unsigned rightImplicitMultTermIndex = 0; rightImplicitMultTermIndex < ADDITIONAL_IMPLICIT_MULT_RIGHT_STRS_LENGTH && !validRightStr; rightImplicitMultTermIndex++)
		{
			string expressionSection = expression.substr(basePosition + 1, additionalImplicitMultRightStrs[rightImplicitMultTermIndex].length());

			if (expressionSection == additionalImplicitMultRightStrs[rightImplicitMultTermIndex])
			{
				validRightStr = true;
			}
		}
	}

	if (validLeftStr)
	{
		for (unsigned thisFunctionIndex = 0; thisFunctionIndex < FUNCTION_NAMES_LENGTH && !leftFunctionName; thisFunctionIndex++)
		{
			int nameSectionStartIndex = (int)basePosition - (functionNames[thisFunctionIndex].length() - 1);

			if (nameSectionStartIndex >= 0)
			{
				if (expression[basePosition + 1] == '(' && expression.substr(nameSectionStartIndex, functionNames[thisFunctionIndex].length()) == functionNames[thisFunctionIndex])
				{
					leftFunctionName = true;
				}
			}
		}

		if (!leftFunctionName)
		{
			for (ExpressionParser::ParsingContext::FunctionIteratorConst thisUserFunction = parsingContext.beginFunctionConst(); thisUserFunction != parsingContext.endFunctionConst() && !leftFunctionName; ++thisUserFunction)
			{
				int nameSectionStartIndex = (int)basePosition - (thisUserFunction.getFunctionSignature().functionName.length() - 1);

				if (nameSectionStartIndex >= 0)
				{
					if (expression[basePosition + 1] == '(' && expression.substr(nameSectionStartIndex, thisUserFunction.getFunctionSignature().functionName.length()) == thisUserFunction.getFunctionSignature().functionName)
					{
						leftFunctionName = true;
					}
				}
			}
		}
	}

	if (validLeftStr && validRightStr && !leftFunctionName && !(isVarChar(expression[basePosition]) && isVarChar(expression[basePosition + 1]))
		&& !(isdigit(expression[basePosition]) && isdigit(expression[basePosition + 1])))
	{
		validImplicitMultExpression = true;
	}

	return validImplicitMultExpression;
}

bool ExpressionParser::parseOperators(string expression,
						  calcObj& result,
						  const ParsingContext& parsingContext,
						  std::string parentExpression) const
{
	vector<ParserOperators::PrecedenceLevel> operatorLists = ParserOperators::getOperatorLists();

	int parenNestLevel = 0,
		braceNestLevel = 0;

	bool foundOperator = false;

	for(unsigned thisLevel = 0; thisLevel < operatorLists.size() && !foundOperator; thisLevel++)
	{
		if(operatorLists[thisLevel].operatorType == ParserOperators::PrecedenceLevel::OperatorType::BINARY_TYPE)
		{
			string currentArgument = "";

			unsigned i = 0;
			vector<ParserOperators::BinaryParsingBlock> parsingBlocks;
			ParserOperators::MultiplicationOperator multOperator;

			while(i < expression.size())
			{
				if(expression[i] == '(')
				{
					parenNestLevel++;
				}
				else if(expression[i] == ')')
				{
					parenNestLevel--;
				}

				if(expression[i] == '{')
				{
					braceNestLevel++;
				}
				else if(expression[i] == '}')
				{
					braceNestLevel--;
				}

				if(parenNestLevel == 0 && braceNestLevel == 0)
				{
					ParserOperators::BinaryOperator* currentOperator = NULL;

					for(unsigned thisOperator = 0; thisOperator < operatorLists[thisLevel].binaryOperators.size() && currentOperator == NULL; thisOperator++)
					{
						string thisOperatorStr = operatorLists[thisLevel].binaryOperators[thisOperator]->getOperatorStr();

						if(i != 0 && i != (int)expression.length() - (int)thisOperatorStr.length() && expression.substr(i, thisOperatorStr.length()) == thisOperatorStr)
						{
							currentOperator = operatorLists[thisLevel].binaryOperators[thisOperator];

							for (unsigned thisOverrideIndex = 0; thisOverrideIndex < currentOperator->overrides.size(); thisOverrideIndex++)
							{
								ParserOperators::BinaryOperator* thisOverride = currentOperator->overrides[thisOverrideIndex].overridingOperator;

								if (expression.substr(i, thisOverride->getOperatorStr().length()) == thisOverride->getOperatorStr())
								{
									if (currentOperator->overrides[thisOverrideIndex].precedenceLevel == thisLevel)
									{
										currentOperator = thisOverride;
									}
									else
									{
										currentOperator = NULL;
									}
								}
							}
						}
					}

					if(currentOperator != NULL)
					{
						if (!(i >= 2 && currentOperator->getOperatorStr() == "-" && expression[i - 1] == 'E' && !isVarChar(expression[i - 2])))
						{
							ParserOperators::BinaryParsingBlock currentBlock = ParserOperators::BinaryParsingBlock();
							currentBlock.terminatingOperator = currentOperator;
							currentBlock.argument = currentArgument;

							parsingBlocks.emplace_back(currentBlock);

							foundOperator = true;
							currentArgument = "";

							i += currentOperator->getOperatorStr().length();
						}
						else
						{
							currentArgument += expression[i];
							i++;
						}
					}
					else
					{
						if (operatorLists[thisLevel].supportsImplicitMult && i < (expression.length() - 1))
						{
							bool validImplicitMultExpression = isValidImplicitTerm(expression, i, parsingContext);

							if (validImplicitMultExpression)
							{
								currentArgument += expression[i];

								ParserOperators::BinaryParsingBlock currentBlock = ParserOperators::BinaryParsingBlock();
								currentBlock.terminatingOperator = &multOperator;
								currentBlock.argument = currentArgument;

								parsingBlocks.emplace_back(currentBlock);

								foundOperator = true;
								currentArgument = "";

								i++;
							}
							else
							{
								currentArgument += expression[i];
								i++;
							}
						}
						else
						{
							currentArgument += expression[i];
							i++;
						}
						
					}
				}
				else
				{
					currentArgument += expression[i];
					i++;
				}
			}

			if(foundOperator)
			{
				ParserOperators::BinaryParsingBlock endingBlock = ParserOperators::BinaryParsingBlock();
				endingBlock.terminatingOperator = NULL;
				endingBlock.argument = currentArgument;

				parsingBlocks.emplace_back(endingBlock);

				if(operatorLists[thisLevel].associativity == ParserOperators::PrecedenceLevel::OperatorAssociativity::LEFT_TO_RIGHT)
				{
					result = parseArithmetic(parsingBlocks[0].argument, parsingContext, expression);

					for(unsigned blockIndex = 1; blockIndex < parsingBlocks.size(); blockIndex++)
					{
						calcObj currentArgumentValue = parseArithmetic(parsingBlocks[blockIndex].argument, parsingContext, expression);

						parsingBlocks[blockIndex - 1].terminatingOperator->applyOperator(result, currentArgumentValue);
					}
				}
				else
				{
					result = parseArithmetic(parsingBlocks[parsingBlocks.size() - 1].argument, parsingContext, expression);

					for(int blockIndex = parsingBlocks.size() - 2; blockIndex >= 0; blockIndex--)
					{
						calcObj currentArgumentValue = parseArithmetic(parsingBlocks[blockIndex].argument, parsingContext, expression);

						parsingBlocks[blockIndex].terminatingOperator->applyOperator(result, currentArgumentValue);
					}
				}
			}
		}
		else
		{
			ParserOperators::UnaryOperator* prefixOperator = NULL;
			ParserOperators::UnaryOperator* postfixOperator = NULL;
			string argumentStr = expression;

			for(unsigned operatorIndex = 0; operatorIndex < operatorLists[thisLevel].unaryOperators.size() && !(prefixOperator != NULL && postfixOperator != NULL); operatorIndex++)
			{
				ParserOperators::UnaryOperator* currentOperator = operatorLists[thisLevel].unaryOperators[operatorIndex];

				if(operatorLists[thisLevel].unaryOperators[operatorIndex]->getOperatorStrMode() == ParserOperators::UnaryOperator::OperatorStrMode::PREFIX_MODE)
				{
					if(expression.substr(0, currentOperator->getOperatorStr().length()) == currentOperator->getOperatorStr())
					{
						prefixOperator = currentOperator;
					}
				}
				else
				{
					if(expression.substr(expression.length() - currentOperator->getOperatorStr().length()) == currentOperator->getOperatorStr())
					{
						postfixOperator = currentOperator;
					}
				}
			}

			if(prefixOperator != NULL)
			{
				argumentStr = argumentStr.substr(prefixOperator->getOperatorStr().length());
			}

			if(postfixOperator != NULL)
			{
				argumentStr = argumentStr.substr(0, argumentStr.length() - postfixOperator->getOperatorStr().length());
			}

			if(prefixOperator != NULL || postfixOperator != NULL)
			{
				foundOperator = true;

				result = parseArithmetic(argumentStr, parsingContext, expression);

				if(prefixOperator != NULL)
				{
					prefixOperator->applyOperator(result);
				}

				if(postfixOperator != NULL)
				{
					postfixOperator->applyOperator(result);
				}
			}
		}
	}

	return foundOperator;
}

calcObj ExpressionParser::parseArithmetic(std::string expression,
						const ParsingContext& parsingContext,
						std::string parentExpression) const
{
	FnResult fnResult;
	calcObj parsedList;

	try
	{
		if(expression == "") // Handle empty strings (may appear in recursive calls)
		{
			// throw exception(("Syntax error in string \"" + parentExpression + "\".").c_str());
			throw SyntaxError(parentExpression);
		}
		else if(isDecimalNumber(expression)) // Handle decimal numbers
		{
			// long expressionPrecision = max((long)significantFigures(expression) + 5, mpfr::mpreal::get_default_prec());
			calcFloat conversionResult;
			stringToFloat(expression, conversionResult);

			return conversionResult;
		}
		else if(parseListString(expression, parsedList, parsingContext))
		{
			return parsedList;
		}
		else if(expression == "e") // Handle Euler's number constant
		{
			return get_e_value();
		}
		else if(expression == "pi") // Handle pi constant
		{
			return get_pi_value();
		}
		else if(parsingContext.variableExists(expression)) // Handle variables
		{
#ifdef MULTIPRECISION
			return calcObj(*(parsingContext.findVariable(expression))).set_precision(mpfr::mpreal::get_default_prec());
#else
			return *(parsingContext.findVariable(expression));
#endif
		}
		else // Try handling expression as function
		{
			fnResult = evalFn(expression, parsingContext);

			if(fnResult.validFn)
			{
				if(fnResult.validDomain)
				{
					return fnResult.result;
				}
				else
				{
					// throw exception(("Domain error in function call " + expression + ".").c_str());
					throw DomainError("function call", expression, fnResult.specificErrorStr);
				}
			}
		}
	

		if(expression[0] == '(' && findClosingChar('(', ')', 0, expression) == expression.length() - 1) // Handle parenthetical expressions (usually appear in recursive calls)
		{
			return parseArithmetic(expression.substr(1, expression.length() - 2), parsingContext, expression);
		}



		/*
		int parenNestLevel = 0, braceNestLevel = 0;

		string currentArg = "";
		char currentOperation = '\0';
		calcObj currentTotal = 0;

		for(int i = 0; i < expression.length(); i++) // Addition/subtraction recursive splitting block
		{
			if(expression[i] == '(')
			{
				parenNestLevel++;
			}
			else if(expression[i] == ')')
			{
				parenNestLevel--;
			}

			if(expression[i] == '{')
			{
				braceNestLevel++;
			}
			else if(expression[i] == '}')
			{
				braceNestLevel--;
			}

			if(parenNestLevel == 0 && braceNestLevel == 0)
			{
				if(expression[i] == '+')
				{
					if(currentOperation == '\0')
					{
						currentTotal = parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
					}
					else if(currentOperation == '+')
					{
						currentTotal += parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
					}
					else
					{
						currentTotal -= parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
					}

					currentOperation = '+';
					currentArg = "";
				}
				else if(expression[i] == '-')
				{
					if(currentOperation == '\0')
					{
						if(currentArg != "")
						{
							currentTotal = parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
						}
						else
						{
							currentTotal = calcObj(0);
						}
					}
					else if(currentOperation == '+')
					{
						currentTotal += parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
					}
					else
					{
						currentTotal -= parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
					}

					currentOperation = '-';
					currentArg = "";
				}
				else
				{
					currentArg += expression[i];
				}
			}
			else
			{
				currentArg += expression[i];
			}
		}

		if(currentOperation == '+')
		{
			currentTotal += parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
		}
		else if(currentOperation == '-')
		{
			currentTotal -= parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
		}

		if(currentOperation != '\0')
		{
			return currentTotal;
		}

		parenNestLevel = braceNestLevel = 0;
		currentArg = "";
		currentOperation = '\0';
		currentTotal = calcObj(0);

		//string prevArg;

		for(int i = 0; i < expression.length(); i++) // Multiplication/division recursive splitting block
		{

			if(parenNestLevel == 0 && braceNestLevel == 0)
			{
				if(i != 0 &&
					expression[i] != '*' && expression[i] != '/' && expression[i] != '^' && expression[i - 1] != '*' && expression[i - 1] != '/' && expression[i - 1] != '^' && expression[i - 1] != '(' && expression[i] != ')' &&
					expression[i] != '.' && expression[i - 1] != '.' && expression[i] != ' ' && expression[i - 1] != ' ' && expression[i] != '%' && expression[i - 1] != '%' && expression[i] != '!' && expression[i - 1] != '!' &&
					((currentArg == "ans" && !settings.enableAnsFn) || (currentArg == "deriv" && !settings.enableDerivFn) || (currentArg == "integral" && !settings.enableIntegralFn) || (find(functionNames.begin(), functionNames.end(), currentArg) == functionNames.end() && functionMap.count(currentArg) == 0)) &&
					(((isdigit(expression[i]) || isdigit(expression[i - 1])) && (isdigit(expression[i]) != isdigit(expression[i - 1]))) || ((isVarChar(expression[i]) || isVarChar(expression[i - 1])) && (isVarChar(expression[i]) != isVarChar(expression[i - 1]))) || (expression[i - 1] == ')' && expression[i] == '(')))
					{
						//return parseArithmetic(expression.substr(0, i)) * parseArithmetic(expression.substr(i, expression.length() - i));
						currentTotal = parseMultDivOperator(currentOperation, currentTotal, currentArg, expression, valueMap, functionMap, settings, resultHistory);

						currentOperation = '*';

						currentArg = expression[i];
				}
				else if(expression[i] == '*')
				{
					//return parseArithmetic(expression.substr(0, i)) * parseArithmetic(expression.substr(i + 1, expression.length() - (i + 1)));
					currentTotal = parseMultDivOperator(currentOperation, currentTotal, currentArg, expression, valueMap, functionMap, settings, resultHistory);

					currentOperation = '*';

					currentArg = "";
				}
				else if(expression[i] == '/')
				{
					//calcFloat denominator = parseArithmetic(expression.substr(i + 1, expression.length() - (i + 1)));
					//if(denominator != 0)
					//{
					//	return parseArithmetic(expression.substr(0, i)) / denominator;
					//}
					//else
					//{
					//	throw exception(("Division by zero error in string \"" + expression + "\".").c_str());
					//}
					currentTotal = parseMultDivOperator(currentOperation, currentTotal, currentArg, expression, valueMap, functionMap, settings, resultHistory);

					currentOperation = '/';
					//prevArg = currentArg;
					currentArg = "";
				}
				else if(expression[i] == '%')
				{
					currentTotal = parseMultDivOperator(currentOperation, currentTotal, currentArg, expression, valueMap, functionMap, settings, resultHistory);

					currentOperation = '%';

					currentArg = "";
				}
				else
				{
					currentArg += expression[i];
				}
			}
			else
			{
				currentArg += expression[i];
			}

			if(expression[i] == '(')
			{
				parenNestLevel++;
			}
			else if(expression[i] == ')')
			{
				parenNestLevel--;
			}

			if(expression[i] == '{')
			{
				braceNestLevel++;
			}
			else if(expression[i] == '}')
			{
				braceNestLevel--;
			}
		}

		if(currentOperation != '\0')
		{
			return parseMultDivOperator(currentOperation, currentTotal, currentArg, expression, valueMap, functionMap, settings, resultHistory);
		}

		parenNestLevel = braceNestLevel = 0;
		currentArg = "";
		currentOperation = '\0';
		currentTotal = calcObj(0);

		for(int i = 0; i < expression.length(); i++) // Exponent recursive splitting block
		{
			if(expression[i] == '(')
			{
				parenNestLevel++;
			}
			else if(expression[i] == ')')
			{
				parenNestLevel--;
			}

			if(expression[i] == '{')
			{
				braceNestLevel++;
			}
			else if(expression[i] == '}')
			{
				braceNestLevel--;
			}

			if(parenNestLevel == 0 && braceNestLevel == 0)
			{
				if(expression[i] == '^')
				{
					if(currentOperation == '\0')
					{
						currentTotal = parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
					}
					else
					{
						currentTotal = pow(currentTotal, parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression));
					}

					currentOperation = '^';
					currentArg = "";
					//return pow(parseArithmetic(expression.substr(0, i)), parseArithmetic(expression.substr(i + 1, expression.length() - (i + 1))));
				}
				else
				{
					currentArg += expression[i];
				}
			}
			else
			{
				currentArg += expression[i];
			}
		}

		if(currentOperation == '^')
		{
			currentTotal = pow(currentTotal, parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression));
		}

		if(currentOperation != '\0')
		{
			return currentTotal;
		} */

		calcObj operatorParseResult;

		bool operatorParseSuccess = parseOperators(expression, operatorParseResult, parsingContext, expression);

		if(operatorParseSuccess)
		{
			return operatorParseResult;
		}

		/* if(expression[expression.size() - 1] == '!')
		{
			calcObj operand = parseArithmetic(expression.substr(0, expression.size() - 1), valueMap, functionMap, settings, resultHistory, expression), result;

			result = factorial(operand.getVerifiedFloat("Factorial type error"));

			if(result == -1.0)
			{
				throw exception(("Domain error in factorial expression \"" + expression + "\".").c_str());
			}
			else
			{
				return result;
			}
		} */
	}
	catch(const type_error& e)
	{
		// throw exception((string(e.what()) + " in expression \"" + expression + "\".").c_str());
		throw ExpressionTypeError(expression, e.what());
	}
	catch(const DivisionByZeroError)
	{
		// throw exception((string(e.what()) + " in expression \"" + expression + "\".").c_str());
		throw ExpressionDivisionByZeroError(expression);
	}
	catch(ExpressionError& e)
	{
		if(e.getExpression() == "")
		{
			e.setExpression(expression);
		}

		throw;
	}
	catch(const exception& e)
	{
		const char* msg = e.what();

		if(strcmp("H0F0 failed to converge", msg) == 0)
		{
			// throw exception(("Overflow error in string \"" + expression + "\".").c_str());
			throw OverflowError(expression);
		}
		else
		{
			throw;
		}
	}

	// throw exception(("Syntax error in string \"" + expression + "\".").c_str());
	throw SyntaxError(expression); // String not handled; throw syntax error
}

/*
calcObj parseMultDivOperator(char currentOperation, calcObj currentTotal, std::string currentArg, std::string expression, const std::map<std::string, calcObj>& valueMap, const std::multimap<std::string, functionDefinition> functionMap, const parseSettings& settings, const std::vector<calcObj>& resultHistory)
{
	if(currentOperation == '\0')
	{
		currentTotal = parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
	}
	else if(currentOperation == '*')
	{
		currentTotal *= parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
	}
	else if (currentOperation == '%')
	{
		calcObj secondArg = parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);

		// if(fmod(currentTotal, calcFloat(1.0)) == calcFloat(0.0) && fmod(secondArg, calcFloat(1.0)) == calcFloat(0.0))
		// {
			// currentTotal = (currentTotal.convert_to<cpp_int>() % secondArg.convert_to<cpp_int>()).convert_to<calcFloat>();
			currentTotal = fmod(currentTotal, secondArg);
		// }
		// else
		// {
		// 	throw exception(("Modulo operator used with non-integer operands in string \"" + expression + "\".").c_str());
		// }
	}
	else
	{
		calcObj denominator = parseArithmetic(currentArg, valueMap, functionMap, settings, resultHistory, expression);
		if(denominator != 0)
		{
			currentTotal /= denominator;
		}
		else
		{
			throw exception(("Division by zero error in string \"" + expression + "\".").c_str());
		}
	}

	return currentTotal;
}
*/

bool ExpressionParser::isIntegerNumber(string expression)
{
	bool validInteger = true;

	if (expression.length() > 0)
	{
		for (unsigned i = 0; i < expression.size() && validInteger; i++)
		{
			if (!isdigit(expression[i]) && !(expression[i] == '-' && i == 0))
			{
				validInteger = false;
			}
		}
	}
	else
	{
		validInteger = false;
	}

	return validInteger;
}

// Function to check whether or not a string represents a number
bool ExpressionParser::isDecimalNumber(string expression)
{
	bool validNumber = true,
		 evaluationCompleted = false,
		 foundDecimalPoint = false;

	if (expression.length() > 0)
	{
		for (unsigned i = 0; i < expression.length() && !evaluationCompleted; i++)
		{
			if (expression[i] == 'E') //(tolower(expression[i]) == 'e')
			{
				if (i > 0)
				{
					string exponentStr = expression.substr(i + 1);

					validNumber = isIntegerNumber(exponentStr);
					evaluationCompleted = true;
				}
				else
				{
					validNumber = false;
				}
			}
			else if (!isdigit(expression[i]) && !(expression[i] == '.' && !foundDecimalPoint) && !(expression[i] == '-' && i == 0))
			{
				validNumber = false;
				evaluationCompleted = true;
			}

			if (expression[i] == '.')
			{
				foundDecimalPoint = true;
			}
		}
	}
	else
	{
		validNumber = false;
	}

	return validNumber;
}

// Function to evaluate built-in mathematical functions (sin, cos, etc.)
ExpressionParser::FnResult ExpressionParser::evalFn(std::string expression, const ParsingContext& parsingContext) const
{
#ifdef MULTIPRECISION
	static gmp_randstate_t randState;
#else
	static mt19937 randEngine;
#endif
	static bool seededEngine = false;

	FnResult result = {false, false, "", 0};

	int openParenIndex = expression.find('(');
	int closeParenIndex = findClosingChar('(', ')', openParenIndex, expression);
	
	if(expression.length() >= 3 && openParenIndex != string::npos && closeParenIndex == expression.length() - 1)
	{
		string fnString = expression.substr(0, openParenIndex);
		string fnExpression = expression.substr(openParenIndex + 1, expression.length() - openParenIndex - 2);
		vector<string> params;
		string currentStr = "";

		{
			unsigned i = 0;
			for (int parenNestLevel = 0, braceNestLevel = 0; i < fnExpression.length(); i++)
			{
				if (fnExpression[i] == '(')
				{
					parenNestLevel++;
				}
				else if (fnExpression[i] == ')')
				{
					parenNestLevel--;
				}

				if (fnExpression[i] == '{')
				{
					braceNestLevel++;
				}
				else if (fnExpression[i] == '}')
				{
					braceNestLevel--;
				}

				if (parenNestLevel == 0 && braceNestLevel == 0 && fnExpression[i] == ',')
				{
					params.push_back(currentStr);
					currentStr = "";
				}
				else
				{
					currentStr += fnExpression[i];
				}
			}
		}

		params.push_back(currentStr);

		if(fnString == "sin" && params.size() == 1)
		{
			string typeErrorMsg = "Sine type error";

			switch(parsingSettings.parseAngleMode)
			{
			case degrees:
				result.result = sin(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg) * get_pi_value() / 180.0);
				break;
			case radians:
				result.result = sin(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				break;
			case gradians:
				result.result = sin(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg) * get_pi_value() / 200.0);
				break;
			}

			result.validFn = true;
			result.validDomain = true;
		}
		else if(fnString == "cos" && params.size() == 1)
		{
			string typeErrorMsg = "Cosine type error";

			switch(parsingSettings.parseAngleMode)
			{
			case degrees:
				result.result = cos(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg) * get_pi_value() / 180.0);
				break;
			case radians:
				result.result = cos(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				break;
			case gradians:
				result.result = cos(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg) * get_pi_value() / 200.0);
				break;
			}

			result.validFn = true;
			result.validDomain = true;
		}
		else if(fnString == "tan" && params.size() == 1)
		{
			string typeErrorMsg = "Tangent type error";

			switch(parsingSettings.parseAngleMode)
			{
			case degrees:
				result.result = tan(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg) * get_pi_value() / 180.0);
				break;
			case radians:
				result.result = tan(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				break;
			case gradians:
				result.result = tan(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg) * get_pi_value() / 200.0);
				break;
			}

			result.validFn = true;
			result.validDomain = true;
		}
		else if(fnString == "arcsin" && params.size() == 1)
		{
			string typeErrorMsg = "Arcsine type error";

			calcFloat expressionVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			result.validFn = true;

			if(expressionVal >= -1 && expressionVal <= 1)
			{
				switch(parsingSettings.parseAngleMode)
				{
				case degrees:
					result.result = asin(expressionVal) * 180.0 / get_pi_value();
					break;
				case radians:
					result.result = asin(expressionVal);
					break;
				case gradians:
					result.result = asin(expressionVal) * 200.0 / get_pi_value();
					break;
				}

				result.validDomain = true;
			}
		}
		else if(fnString == "arccos" && params.size() == 1)
		{
			string typeErrorMsg = "Arccosine type error";

			calcFloat expressionVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			result.validFn = true;

			if(expressionVal >= -1 && expressionVal <= 1)
			{
				switch(parsingSettings.parseAngleMode)
				{
				case degrees:
					result.result = acos(expressionVal) * 180.0 / get_pi_value();
					break;
				case radians:
					result.result = acos(expressionVal);
					break;
				case gradians:
					result.result = acos(expressionVal) * 200.0 / get_pi_value();
					break;
				}

				result.validDomain = true;
			}
		}
		else if(fnString == "arctan" && params.size() == 1)
		{
			string typeErrorMsg = "Arctangent type error";

			switch(parsingSettings.parseAngleMode)
			{
			case degrees:
				result.result = atan(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg)) * 180.0 / get_pi_value();
				break;
			case radians:
				result.result = atan(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				break;
			case gradians:
				result.result = atan(parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg)) * 200.0 / get_pi_value();
				break;
			}

			result.validFn = true;
			result.validDomain = true;
		}
		else if(fnString == "sinh" && params.size() == 1)
		{
			string typeErrorMsg = "Hyperbolic sine type error";

			calcFloat argument = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validDomain = result.validFn = true;

			result.result = sinh(argument);
		}
		else if(fnString == "cosh" && params.size() == 1)
		{
			string typeErrorMsg = "Hyperbolic cosine type error";

			calcFloat argument = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validDomain = result.validFn = true;

			result.result = cosh(argument);
		}
		else if(fnString == "tanh" && params.size() == 1)
		{
			string typeErrorMsg = "Hyperbolic tangent type error";

			calcFloat argument = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validDomain = result.validFn = true;

			result.result = tanh(argument);
		}
		else if(fnString == "arcsinh" && params.size() == 1)
		{
			string typeErrorMsg = "Hyperbolic arcsine type error";

			calcFloat argument = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validDomain = result.validFn = true;

			result.result = log(argument + sqrt(1 + (argument * argument)));
		}
		else if(fnString == "arccosh" && params.size() == 1)
		{
			string typeErrorMsg = "Hyperbolic arccosine type error";

			calcFloat argument = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = true;

			if(argument >= 1)
			{
				result.validDomain = true;
				result.result = log(argument + (sqrt(argument + 1) * sqrt(argument - 1)));
			}
		}
		else if(fnString == "arctanh" && params.size() == 1)
		{
			string typeErrorMsg = "Hyperbolic arctangent type error";

			calcFloat argument = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = true;

			if(argument > -1 && argument < 1)
			{
				result.validDomain = true;
				result.result = (log(1 + argument) - log(1 - argument)) / 2;
			}
		}
		else if(fnString == "sqrt" && params.size() == 1)
		{
			string typeErrorMsg = "Square root type error";

			calcFloat expressionVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			result.validFn = true;

			if(expressionVal >= 0)
			{
				result.validDomain = true;
				result.result = sqrt(expressionVal);
			}
		}
		else if(fnString == "ln" && params.size() == 1)
		{
			string typeErrorMsg = "Natural log type error";

			calcFloat expressionVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			result.validFn = true;
			
			if(expressionVal > 0)
			{
				result.validDomain = true;
				result.result = log(expressionVal);
			}
		}
		else if(fnString == "log" && params.size() == 1)
		{
			string typeErrorMsg = "Common log type error";

			calcFloat expressionVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			result.validFn = true;
			
			if(expressionVal > 0)
			{
				result.validDomain = true;
				result.result = log10(expressionVal);
			}
		}
		else if(fnString == "logbase" && params.size() == 2)
		{
			string typeErrorMsg = "Logbase type error";

			calcFloat arg1Val = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
				   arg2Val = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			result.validFn = true;

			if(arg1Val > 0 && (arg2Val > 0 && arg2Val != 1.0))
			{
				result.validDomain = true;
				result.result = (log10(arg1Val) / log10(arg2Val));
			}
		}
		else if(fnString == "ans" && parsingSettings.enableAnsFn && params.size() == 1)
		{
			string typeErrorMsg = "Ans type error";
			result.validFn = true;

			calcFloat expressionVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			unsigned expressionValInt;

			try
			{
				expressionValInt = floatToUnsigned(expressionVal);
				result.validDomain = true;
			}
			catch (NumericConversionError ex)
			{
				result.validDomain = false;
			}

			if (result.validDomain)
			{
				if (expressionValInt >= 1 && resultHistory.size() >= expressionValInt)
				{
					result.validDomain = true;

					result.result = resultHistory[resultHistory.size() - expressionValInt];

#ifdef MULTIPRECISION
					result.result.set_precision(mpfr::mpreal::get_default_prec());
#endif
				}
				else
				{
					result.validDomain = false;
				}
			}
		}
		else if(fnString == "deriv" && parsingSettings.enableDerivFn && params.size() == 3)
		{
			string typeErrorMsg = "Derivative type error";

			calcFloat varValue = parseArithmetic(params[2], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			result.validFn = result.validDomain = true;
			
			result.result = deriv(params[0], params[1], varValue, 1.0e-4, parsingContext, expression);
		}
		else if(fnString == "deriv" && parsingSettings.enableDerivFn && params.size() == 4)
		{
			string typeErrorMsg = "Derivative type error";
			unsigned derivNum;

			result.validFn = true;

			calcFloat varValue = parseArithmetic(params[2], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			try
			{
				derivNum = floatToUnsigned(parseArithmetic(params[3], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				result.validDomain = true;
			}
			catch (NumericConversionError ex)
			{
				result.validDomain = false;
			}

			if (result.validDomain)
			{
				result.result = deriv(params[0], params[1], varValue, 1.0e-4 /*1.0e-6 * pow(10.0, (int)derivNum) */, parsingContext, expression, derivNum);
			}
		}
		else if(fnString == "deriv" && parsingSettings.enableDerivFn && params.size() == 5)
		{
			string typeErrorMsg = "Derivative type error";
			unsigned derivNum;

			result.validFn = true;

			calcFloat varValue = parseArithmetic(params[2], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
				   delta = parseArithmetic(params[4], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			try
			{
				derivNum = floatToUnsigned(parseArithmetic(params[3], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				result.validDomain = true;
			}
			catch (NumericConversionError ex)
			{
				result.validDomain = false;
			}

			if (result.validDomain)
			{
				result.result = deriv(params[0], params[1], varValue, delta, parsingContext, expression, derivNum);
			}
		}
		else if(fnString == "integral" && parsingSettings.enableIntegralFn && params.size() == 4)
		{
			string typeErrorMsg = "Integral type error";

			calcFloat lower = parseArithmetic(params[2], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
				   upper = parseArithmetic(params[3], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = result.validDomain = true;

#ifdef MULTIPRECISION
			result.result = gaussIntegral(params[0], params[1], lower, upper,/* 1.0e-4 * (upper - lower),*/ 50, parsingContext, expression);
#else
			result.result = gaussIntegral(params[0], params[1], lower, upper,/* 1.0e-4 * (upper - lower),*/ 10, parsingContext, expression);
#endif
			// cout << "Trapezoidal result: " << sintegral(params[0], params[1], lower, upper, 1.0e-4 * (upper - lower), valueMap, settings, resultHistory, expression) << endl;
		}
		else if (fnString == "solve" && (params.size() == 4 || params.size() == 5))
		{
			string expression = params[0],
				   variable = params[1];
			vector<calcFloat> results;
			
			result.validFn = result.validDomain = true;

			calcFloat lBound = this->parseArithmetic(params[2], parsingContext).getVerifiedFloat("Solver lower-bound type error"),
					  hBound = this->parseArithmetic(params[3], parsingContext).getVerifiedFloat("Solver upper-bound type error");

			unsigned intervals;

			if (params.size() == 5)
			{
				calcFloat intervalsFloat = this->parseArithmetic(params[4], parsingContext).getVerifiedFloat("Solver interval number type error");

				try
				{
					intervals = floatToUnsigned(intervalsFloat);
				}
				catch (NumericConversionError)
				{
					result.validDomain = false;
					result.specificErrorStr = "invalid number of intervals specified";
				}
			}
			else
			{
				intervals = 16;
			}
			
			if (result.validDomain)
			{
				this->bisectionSolve(expression, variable, lBound, hBound, results, intervals, parsingContext, expression);
				result.result.set_list(results);
			}
		}
		else if(fnString == "abs" && params.size() == 1)
		{
			string typeErrorMsg = "Absolute value type error";

			calcFloat paramVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = result.validDomain = true;

			result.result = fabs(paramVal);
		}
		else if(fnString == "ceil" && params.size() == 1)
		{
			string typeErrorMsg = "Ceil type error";

			calcFloat paramVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = result.validDomain = true;

			result.result = ceil(paramVal);
		}
		else if(fnString == "ceil" && params.size() == 2)
		{
			string typeErrorMsg = "Ceil value type error";

			calcFloat paramVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
					  multipleVal  = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = true;

			if(multipleVal == 0.0)
			{
				result.validDomain = false;
			}
			else
			{
				result.validDomain = true;

				result.result = ceil(paramVal / multipleVal) * multipleVal;
			}
		}
		else if(fnString == "floor" && params.size() == 1)
		{
			string typeErrorMsg = "Floor value type error";

			calcFloat paramVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = result.validDomain = true;

			result.result = floor(paramVal);
		}
		else if(fnString == "floor" && params.size() == 2)
		{
			string typeErrorMsg = "Floor value type error";

			calcFloat paramVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
					  multipleVal  = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = true;

			if(multipleVal == 0.0)
			{
				result.validDomain = false;
			}
			else
			{
				result.validDomain = true;

				result.result = floor(paramVal / multipleVal) * multipleVal;
			}
		}
		else if(fnString == "round" && params.size() == 1)
		{
			string typeErrorMsg = "Round value type error";

			calcFloat paramVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = result.validDomain = true;

			result.result = round(paramVal);
		}
		else if(fnString == "round" && params.size() == 2)
		{
			string typeErrorMsg = "Round value type error";

			calcFloat paramVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
					  multipleVal  = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			result.validFn = true;

			if(multipleVal == 0.0)
			{
				result.validDomain = false;
			}
			else
			{
				result.validDomain = true;

				result.result = round(paramVal / multipleVal) * multipleVal;
			}
		}
		else if(fnString == "primeFct" && params.size() == 1)
		{
			calcFloat factorNum;
			calcSignedInt factorNumInt;
			bool nonInteger = false;
			string typeErrorMsg = "Prime factorization type error";
			vector<calcFloat> resultFactors;

			result.validFn = true;

			factorNum = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			try
			{
				floatToInt(factorNum, factorNumInt);
			}
			catch(exception ex)
			{
				nonInteger = true;
			}

			if(nonInteger || factorNumInt <= 1)
			{
				result.validDomain = false;
			}
			else
			{
				result.validDomain = true;

				ParserMathematicalFunctions::primeFactorization(resultFactors, factorNumInt);

				result.result.set_list(resultFactors);
			}
		}
		else if(fnString == "isPrime" && params.size() == 1)
		{
			calcFloat checkNum;
			calcSignedInt checkNumInt;
			bool nonInteger = false;
			string typeErrorMsg = "Prime checking type error";

			result.validFn = true;

			checkNum = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			try
			{
				floatToInt(checkNum, checkNumInt);
			}
			catch(NumericConversionError ex)
			{
				nonInteger = true;
			}

			if(nonInteger || checkNumInt <= 1)
			{
				result.validDomain = false;
			}
			else
			{
				result.validDomain = true;

				result.result = boolToFloat(ParserMathematicalFunctions::isPrime(checkNumInt));
			}
		}
		else if(fnString == "gcd" && params.size() == 2)
		{
			result.validFn = true;

			string typeErrorMsg = "Greatest common denominator type error";
			bool nonInteger = false;

			calcFloat firstVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
					  secondVal = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			calcSignedInt firstValInt, secondValInt, resultInt;

			if(firstVal <= 0 || secondVal <= 0)
			{
				result.validDomain = false;
			}
			else
			{
				try
				{
					floatToInt(firstVal, firstValInt);
					floatToInt(secondVal, secondValInt);

					// cout << firstValInt << ' ' << secondValInt << endl;
				}
				catch(NumericConversionError ex)
				{
					nonInteger = true;
				}

				if(nonInteger)
				{
					result.validDomain = false;
				}
				else
				{
					result.validDomain = true;

#ifdef MULTIPRECISION
					resultInt = gcd(firstValInt, secondValInt);
#else
					resultInt = ParserMathematicalFunctions::greatestCommonDenominator(firstValInt, secondValInt);
#endif

					calcFloat buffer;
					intToFloat(resultInt, buffer);

					result.result = buffer;
				}
			}
		}
		else if(fnString == "lcm" && params.size() == 2)
		{
			result.validFn = true;

			string typeErrorMsg = "Least common multiple type error";
			bool nonInteger = false;

			calcFloat firstVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
					  secondVal = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			calcSignedInt firstValInt, secondValInt, resultInt;

			if(firstVal <= 0 || secondVal <= 0)
			{
				result.validDomain = false;
			}
			else
			{
				try
				{
					floatToInt(firstVal, firstValInt);
					floatToInt(secondVal, secondValInt);

					// cout << firstValInt << ' ' << secondValInt << endl;
				}
				catch(NumericConversionError ex)
				{
					nonInteger = true;
				}

				if(nonInteger)
				{
					result.validDomain = false;
				}
				else
				{
					result.validDomain = true;

#ifdef MULTIPRECISION
					resultInt = lcm(firstValInt, secondValInt);
#else
					resultInt = ParserMathematicalFunctions::leastCommonMultiple(firstValInt, secondValInt);
#endif

					calcFloat buffer;
					intToFloat(resultInt, buffer);

					result.result = buffer;
				}
			}
		}
		else if(fnString == "rand" && params.size() == 1 && params[0] == "")
		{
			result.validFn = result.validDomain = true;
#ifdef MULTIPRECISION
			calcFloat floatResult;

			if(!seededEngine)
			{
				seededEngine = true;

				gmp_randinit_mt(randState);
			}

			mpfr_urandomb(floatResult.mpfr_ptr(), randState);

			result.result = floatResult;
#else
			if (!seededEngine)
			{
				long long tickCount = chrono::high_resolution_clock::now().time_since_epoch().count();
				unsigned seedValue = static_cast<unsigned>(tickCount % (static_cast<long long>(numeric_limits<unsigned>::max()) + 1));

				seededEngine = true;
				randEngine.seed(seedValue);
			}

			uniform_real_distribution<calcFloat> distributionGenerator(0.0, 1.0);
			result.result = distributionGenerator(randEngine);
#endif
		}
		else if(fnString == "randInt" && params.size() == 2)
		{
			result.validFn = true;

			bool nonInteger = false;
			calcSignedInt minValInt, maxValInt;
			calcFloat minVal, maxVal;
			string typeErrorMsg = "Random integer type error";

			minVal = parseArithmetic(params[0], parsingContext, expression).getVerifiedFloat(typeErrorMsg);
			maxVal = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			if(minVal > maxVal)
			{
				result.validDomain = false;
			}
			else
			{
				try
				{
					floatToInt(minVal, minValInt);
					floatToInt(maxVal, maxValInt);
				}
				catch(NumericConversionError ex)
				{
					nonInteger = true;
				}

				if(nonInteger)
				{
					result.validDomain = false;
				}
				else
				{
					result.validDomain = true;

#ifdef MULTIPRECISION
					calcSignedInt randNum, randRange;

					if(!seededEngine)
					{
						seededEngine = true;

						gmp_randinit_mt(randState);
					}

					randRange = (maxValInt - minValInt + 1);
					mpz_urandomm(randNum.backend().data(), randState, randRange.backend().data());

					randNum += minValInt;

					calcFloat floatResult;
					intToFloat(randNum, floatResult);

					result.result = floatResult;
#else
					if (!seededEngine)
					{
						long long tickCount = chrono::high_resolution_clock::now().time_since_epoch().count();
						unsigned seedValue = static_cast<unsigned>(tickCount % (static_cast<long long>(numeric_limits<unsigned>::max()) + 1));

						seededEngine = true;
						randEngine.seed(seedValue);
					}

					calcFloat floatResult;

					uniform_int_distribution<calcSignedInt> distributionGenerator(minValInt, maxValInt);
					intToFloat(distributionGenerator(randEngine), floatResult);
					result.result = floatResult;
#endif
				}
			}
		}
		else if(fnString == "seq" && (params.size() == 4 || params.size() == 5))
		{
			vector<calcObj> values;
			vector<calcFloat> floatValues;

			string typeErrorMsg = "Sequence type error";

			result.validFn = true;

			calcFloat lower = parseArithmetic(params[2], parsingContext, expression).getVerifiedFloat(typeErrorMsg),
					  upper = parseArithmetic(params[3], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

			if(params.size() == 5)
			{
				calcFloat delta = parseArithmetic(params[4], parsingContext, expression).getVerifiedFloat(typeErrorMsg);

				try
				{
					evalSeq(values, params[0], params[1], lower, upper, parsingContext, expression, false, delta);
					result.validDomain = true;
				}
				catch (DomainError ex)
				{
					result.validDomain = false;
					result.specificErrorStr = ex.getSpecificInformation();
				}
			}
			else
			{
				try
				{
					evalSeq(values, params[0], params[1], lower, upper, parsingContext, expression);
					result.validDomain = true;
				}
				catch (DomainError ex)
				{
					result.validDomain = false;
					result.specificErrorStr = ex.getSpecificInformation();
				}
			}

			toVerifiedFloatVector(values, floatValues, typeErrorMsg);

			result.result.set_list(floatValues);
		}
		else if(fnString == "foreach" && params.size() == 2)
		{
			calcObj listObj = parseArithmetic(params[0], parsingContext, expression),
					newObj;

			vector<calcFloat> listValues, newValues;

			ParsingContext functionContext = ParsingContext(&parsingContext);

			result.validFn = result.validDomain = true;

			string valueTypeErrorStr = "Foreach value type error",
				   argumentTypeErrorStr = "Foreach argument type error";

			if(listObj.get_type() == TYPE_LIST)
			{
				listObj.get_list(listValues);

				newValues.resize(listValues.size());

				for(unsigned i = 0; i < listValues.size(); i++)
				{
					functionContext.setVariable("index", calcObj(i));
					functionContext.setVariable("value", listValues[i]);
					functionContext.setVariable("length", calcObj(listValues.size()));

					newValues[i] = parseArithmetic(params[1], functionContext, expression).getVerifiedFloat(valueTypeErrorStr);
				}

				result.result.set_list(newValues);
			}
			else
			{
				throw ExpressionTypeError(expression, argumentTypeErrorStr);
			}
		}
		else if(fnString == "getIndex" && params.size() == 2)
		{
			calcObj listObj = parseArithmetic(params[0], parsingContext, expression);

			result.validFn = true;

			string valueTypeErrorStr = "Getindex value type error",
				   argumentTypeErrorStr = "Getindex argument type error";

			if(listObj.get_type() == TYPE_LIST)
			{
				calcFloat index = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(argumentTypeErrorStr);
				unsigned intIndex;
				
				try
				{
					intIndex = floatToUnsigned(index);
					result.validDomain = true;
				}
				catch (NumericConversionError ex)
				{
					result.validDomain = false;
				}

				if (result.validDomain)
				{
					result.result = listObj.get_list_index(intIndex);
				}
			}
			else
			{
				throw ExpressionTypeError(expression, argumentTypeErrorStr);
			}
		}
		else if(fnString == "setIndex" && params.size() == 3)
		{
			calcObj listObj = parseArithmetic(params[0], parsingContext, expression);

			result.validFn = true;

			string valueTypeErrorStr = "Setindex value type error",
				   argumentTypeErrorStr = "Setindex argument type error";

			if(listObj.get_type() == TYPE_LIST)
			{
				calcFloat index = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(argumentTypeErrorStr), newVal;
				unsigned intIndex;

				try
				{
					intIndex = floatToUnsigned(index);
					result.validDomain = true;
				}
				catch (NumericConversionError ex)
				{
					result.validDomain = false;
				}

				newVal = parseArithmetic(params[2], parsingContext, expression).getVerifiedFloat(argumentTypeErrorStr);

				if (result.validDomain)
				{
					listObj.set_list_index(intIndex, newVal);

					result.result = listObj;
				}
			}
			else
			{
				throw ExpressionTypeError(expression, argumentTypeErrorStr);
			}
		}
		else if(fnString == "length" && params.size() == 1)
		{
			calcObj listObj = parseArithmetic(params[0], parsingContext, expression);

			result.validFn = result.validDomain = true;

			string typeErrorStr = "Length type error";

			if(listObj.get_type() == TYPE_LIST)
			{
				result.result = calcObj(listObj.get_list_length());
			}
			else
			{
				throw ExpressionTypeError(expression, typeErrorStr);
			}
		}
		else if(fnString == "concat" && params.size() >= 2)
		{
			calcObj concatResult = parseArithmetic(params[0], parsingContext, expression);

			result.validFn = result.validDomain = true;

			for(unsigned i = 1; i < params.size(); i++)
			{
				concatResult.concat_list(parseArithmetic(params[i], parsingContext, expression));
			}

			result.result = concatResult;
		}
		else if(fnString == "sublist" && params.size() == 3)
		{
			calcObj origList = parseArithmetic(params[0], parsingContext, expression);

			string indexTypeErrorStr = "Sublist index type error";

			calcFloat startIndex = parseArithmetic(params[1], parsingContext, expression).getVerifiedFloat(indexTypeErrorStr),
					  endIndex = parseArithmetic(params[2], parsingContext, expression).getVerifiedFloat(indexTypeErrorStr);

			unsigned startIndexInt,
					 endIndexInt;

			try
			{
				startIndexInt = floatToUnsigned(startIndex);
				endIndexInt = floatToUnsigned(endIndex);

				result.validDomain = true;
			}
			catch (NumericConversionError ex)
			{
				result.validDomain = false;
			}

			if (result.validDomain)
			{
				vector<calcFloat> sublistBuffer;

				result.validFn = result.validDomain = true;

				origList.sublist(sublistBuffer, startIndexInt, endIndexInt);

				result.result.set_list(sublistBuffer);
			}
		}
		else if(fnString == "filter" && params.size() == 2)
		{
			calcObj listObj = parseArithmetic(params[0], parsingContext, expression),
					newObj;

			vector<calcFloat> listValues, newValues;

			ParsingContext functionContext = ParsingContext(&parsingContext);

			result.validFn = result.validDomain = true;

			string valueTypeErrorStr = "Filter value type error",
				   argumentTypeErrorStr = "Filter argument type error";

			if(listObj.get_type() == TYPE_LIST)
			{
				listObj.get_list(listValues);

				for(unsigned i = 0; i < listValues.size(); i++)
				{
					functionContext.setVariable("index", calcObj(i));
					functionContext.setVariable("value", listValues[i]);
					functionContext.setVariable("length", calcObj(listValues.size()));

					calcFloat keepVal = parseArithmetic(params[1], functionContext, expression).getVerifiedFloat(valueTypeErrorStr);

					if(keepVal != 0.0)
					{
						newValues.push_back(listValues[i]);
					}
				}

				result.result.set_list(newValues);
			}
			else
			{
				throw ExpressionTypeError(expression, argumentTypeErrorStr);
			}
		}
		else if(fnString == "avg" && params.size() >= 1)
		{
			vector<calcFloat> parsedParams;

			string typeErrorMsg = "Average type error";

			result.validFn = true;

			calcObj firstParam = parseArithmetic(params[0], parsingContext, expression);

			if(firstParam.get_type() == TYPE_LIST)
			{
				vector<calcFloat> avgList;
				
				firstParam.get_list(avgList);

				if(avgList.size() > 0)
				{
					result.validDomain = true;

					result.result = ParserMathematicalFunctions::average(avgList);
				}
				else
				{
					result.validDomain = false;
				}
			}
			else
			{
				result.validDomain = true;

				parsedParams.push_back(firstParam.getVerifiedFloat(typeErrorMsg));

				for(unsigned i = 1; i < params.size(); i++)
				{
					parsedParams.push_back(parseArithmetic(params[i], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				}

				result.result = ParserMathematicalFunctions::average(parsedParams);
			}
		}
		//else if(fnString == "avgseq" && (params.size() == 4 || params.size() == 5))
		//{
		//	vector<calcObj> values;
		//	vector<calcFloat> floatValues;

		//	string typeErrorMsg = "Sequence average type error";

		//	result.validFn = result.validDomain = true;

		//	calcFloat lower = parseArithmetic(params[2], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg),
		//			  upper = parseArithmetic(params[3], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg);

		//	if(params.size() == 5)
		//	{
		//		calcFloat delta = parseArithmetic(params[4], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg);

		//		evalSeq(values, params[0], params[1], lower, upper, valueMap, settings, resultHistory, expression, delta);
		//	}
		//	else
		//	{
		//		evalSeq(values, params[0], params[1], lower, upper, valueMap, settings, resultHistory, expression);
		//	}

		//	toVerifiedFloatVector(values, floatValues, typeErrorMsg);

		//	result.result = average(floatValues);
		//}
		//else if(fnString == "sumseq" && (params.size() == 4 || params.size() == 5))
		//{
		//	vector<calcObj> values;
		//	calcObj total = 0.0;

		//	string typeErrorMsg = "Sequence sum type error";

		//	result.validFn = result.validDomain = true;

		//	calcFloat lower = parseArithmetic(params[2], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg),
		//			  upper = parseArithmetic(params[3], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg);

		//	if(params.size() == 5)
		//	{
		//		calcFloat delta = parseArithmetic(params[4], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg);

		//		evalSeq(values, params[0], params[1], lower, upper, valueMap, settings, resultHistory, expression, delta);
		//	}
		//	else
		//	{
		//		evalSeq(values, params[0], params[1], lower, upper, valueMap, settings, resultHistory, expression);
		//	}

		//	for(int i = 0; i < values.size(); i++)
		//	{
		//		total += values[i];

		//		// cout << "Adding " << values[i] << ": " << total << endl;
		//	}

		//	result.result = total;
		//}
		else if(fnString == "sum" && params.size() >= 1)
		{
			vector<calcFloat> parsedParams;

			string typeErrorMsg = "Sum type error";

			result.validFn = true;

			calcObj firstParam = parseArithmetic(params[0], parsingContext, expression);

			if(firstParam.get_type() == TYPE_LIST)
			{
				vector<calcFloat> sumList;
				
				firstParam.get_list(sumList);

				if(sumList.size() > 0)
				{
					result.validDomain = true;

					result.result = ParserMathematicalFunctions::sum(sumList);
				}
				else
				{
					result.validDomain = false;
				}
			}
			else
			{
				result.validDomain = false;

				parsedParams.push_back(firstParam.getVerifiedFloat(typeErrorMsg));

				for(unsigned i = 1; i < params.size(); i++)
				{
					parsedParams.push_back(parseArithmetic(params[i], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				}

				result.result = ParserMathematicalFunctions::sum(parsedParams);
			}
		}
		else if(fnString == "product" && params.size() >= 1)
		{
			vector<calcFloat> parsedParams;

			string typeErrorMsg = "Product type error";

			result.validFn = true;

			calcObj firstParam = parseArithmetic(params[0], parsingContext, expression);

			if(firstParam.get_type() == TYPE_LIST)
			{
				vector<calcFloat> productList;
				
				firstParam.get_list(productList);

				if(productList.size() > 0)
				{
					result.validDomain = true;

					result.result = ParserMathematicalFunctions::product(productList);
				}
				else
				{
					result.validDomain = false;
				}
			}
			else
			{
				result.validDomain = false;

				parsedParams.push_back(firstParam.getVerifiedFloat(typeErrorMsg));

				for(unsigned i = 1; i < params.size(); i++)
				{
					parsedParams.push_back(parseArithmetic(params[i], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				}

				result.result = ParserMathematicalFunctions::product(parsedParams);
			}
		}
		else if(fnString == "stddev" && params.size() >= 1)
		{
			vector<calcFloat> parsedParams;

			string typeErrorMsg = "Standard deviation type error";

			result.validFn = true;

			calcObj firstParam = parseArithmetic(params[0], parsingContext, expression);

			if(firstParam.get_type() == TYPE_LIST)
			{
				vector<calcFloat> stddevList;
				
				firstParam.get_list(stddevList);

				if(stddevList.size() > 0)
				{
					result.validDomain = true;

					result.result = ParserMathematicalFunctions::stddev(stddevList);
				}
				else
				{
					result.validDomain = false;
				}
			}
			else
			{
				result.validDomain = true;

				parsedParams.push_back(firstParam.getVerifiedFloat(typeErrorMsg));

				for(unsigned i = 1; i < params.size(); i++)
				{
					parsedParams.push_back(parseArithmetic(params[i], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				}

				result.result = ParserMathematicalFunctions::stddev(parsedParams);
			}
		}
		else if(fnString == "variance" && params.size() >= 1)
		{
			vector<calcFloat> parsedParams;

			string typeErrorMsg = "Variance type error";

			result.validFn = true;

			calcObj firstParam = parseArithmetic(params[0], parsingContext, expression);

			if(firstParam.get_type() == TYPE_LIST)
			{
				vector<calcFloat> varianceList;
				
				firstParam.get_list(varianceList);

				if(varianceList.size() > 0)
				{
					result.validDomain = true;

					result.result = ParserMathematicalFunctions::variance(varianceList);
				}
				else
				{
					result.validDomain = false;
				}
			}
			else
			{
				result.validDomain = true;

				parsedParams.push_back(firstParam.getVerifiedFloat(typeErrorMsg));

				for(unsigned i = 1; i < params.size(); i++)
				{
					parsedParams.push_back(parseArithmetic(params[i], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				}

				result.result = ParserMathematicalFunctions::variance(parsedParams);
			}
		}
		else if(fnString == "max" && params.size() >= 1)
		{
			vector<calcFloat> parsedParams;

			string typeErrorMsg = "Max type error";

			result.validFn = true;

			calcObj firstParam = parseArithmetic(params[0], parsingContext, expression);

			if(firstParam.get_type() == TYPE_LIST)
			{
				vector<calcFloat> maxList;
				
				firstParam.get_list(maxList);

				if(maxList.size() > 0)
				{
					result.validDomain = true;

					result.result = ParserMathematicalFunctions::max(maxList);
				}
				else
				{
					result.validDomain = false;
				}
			}
			else
			{
				result.validDomain = true;

				parsedParams.push_back(firstParam.getVerifiedFloat(typeErrorMsg));

				for(unsigned i = 1; i < params.size(); i++)
				{
					parsedParams.push_back(parseArithmetic(params[i], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				}

				result.result = ParserMathematicalFunctions::max(parsedParams);
			}
		}
		else if(fnString == "min" && params.size() >= 1)
		{
			vector<calcFloat> parsedParams;

			string typeErrorMsg = "Min type error";

			result.validFn = true;

			calcObj firstParam = parseArithmetic(params[0], parsingContext, expression);

			if(firstParam.get_type() == TYPE_LIST)
			{
				vector<calcFloat> minList;
				
				firstParam.get_list(minList);

				if(minList.size() > 0)
				{
					result.validDomain = true;

					result.result = ParserMathematicalFunctions::min(minList);
				}
				else
				{
					result.validDomain = false;
				}
			}
			else
			{
				result.validDomain = true;

				parsedParams.push_back(firstParam.getVerifiedFloat(typeErrorMsg));

				for(unsigned i = 1; i < params.size(); i++)
				{
					parsedParams.push_back(parseArithmetic(params[i], parsingContext, expression).getVerifiedFloat(typeErrorMsg));
				}

				result.result = ParserMathematicalFunctions::min(parsedParams);
			}
		}
		/*else if(fnString == "stddevseq" && (params.size() == 4 || params.size() == 5))
		{
			vector<calcObj> values;
			vector<calcFloat> floatValues;

			string typeErrorMsg = "Sequence standard deviation type error";

			result.validFn = result.validDomain = true;

			calcFloat lower = parseArithmetic(params[2], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg),
					  upper = parseArithmetic(params[3], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg);

			if(params.size() == 5)
			{
				calcFloat delta = parseArithmetic(params[4], valueMap, settings, resultHistory, expression).getVerifiedFloat(typeErrorMsg);

				evalSeq(values, params[0], params[1], lower, upper, valueMap, settings, resultHistory, expression, delta);
			}
			else
			{
				evalSeq(values, params[0], params[1], lower, upper, valueMap, settings, resultHistory, expression);
			}

			toVerifiedFloatVector(values, floatValues, typeErrorMsg);

			result.result = stddev(floatValues);
		}*/
		else if(fnString == "or" && params.size() >= 2)
		{
			bool orResult = false;

			result.validFn = result.validDomain = true;

			for(unsigned i = 0; (i < params.size()) && !orResult; i++)
			{
				orResult = (parseArithmetic(params[i], parsingContext, expression) != 0);
			}

			result.result = boolToFloat(orResult);
		}
		else if(fnString == "and" && params.size() >= 2)
		{
			bool andResult = true;

			result.validFn = result.validDomain = true;

			for(unsigned i = 0; (i < params.size()) && andResult; i++)
			{
				andResult = (parseArithmetic(params[i], parsingContext, expression) != 0);
			}

			result.result = boolToFloat(andResult);
		}
		else if(fnString == "not" && params.size() == 1)
		{
			bool notResult = (parseArithmetic(params[0], parsingContext, expression) == 0);

			result.validFn = result.validDomain = true;

			result.result = boolToFloat(notResult);
		}
		/*
		else if(fnString == "lte" && params.size() == 2)
		{
			calcObj arg1 = parseArithmetic(params[0], parsingContext, expression),
					  arg2 = parseArithmetic(params[1], parsingContext, expression);

			result.validFn = result.validDomain = true;

			result.result = boolToFloat(arg1 <= arg2);
		}
		else if(fnString == "lt" && params.size() == 2)
		{
			calcObj arg1 = parseArithmetic(params[0], parsingContext, expression),
					  arg2 = parseArithmetic(params[1], parsingContext, expression);

			result.validFn = result.validDomain = true;

			result.result = boolToFloat(arg1 < arg2);
		}
		else if(fnString == "gte" && params.size() == 2)
		{
			calcObj arg1 = parseArithmetic(params[0], parsingContext, expression),
					  arg2 = parseArithmetic(params[1], parsingContext, expression);

			result.validFn = result.validDomain = true;

			result.result = boolToFloat(arg1 >= arg2);
		}
		else if(fnString == "gt" && params.size() == 2)
		{
			calcObj arg1 = parseArithmetic(params[0], parsingContext, expression),
					  arg2 = parseArithmetic(params[1], parsingContext, expression);

			result.validFn = result.validDomain = true;

			result.result = boolToFloat(arg1 > arg2);
		}
		else if(fnString == "eq" && params.size() == 2)
		{
			calcObj arg1 = parseArithmetic(params[0], parsingContext, expression),
					  arg2 = parseArithmetic(params[1], parsingContext, expression);

			result.validFn = result.validDomain = true;

			result.result = boolToFloat(arg1 == arg2);
		}
		else if(fnString == "ne" && params.size() == 2)
		{
			calcObj arg1 = parseArithmetic(params[0], parsingContext, expression),
					  arg2 = parseArithmetic(params[1], parsingContext, expression);

			result.validFn = result.validDomain = true;

			result.result = boolToFloat(arg1 != arg2);
		}
		*/
		else if(fnString == "if" && params.size() == 3)
		{
			bool conditionVal = (parseArithmetic(params[0], parsingContext, expression) != 0);

			result.validFn = result.validDomain = true;

			if(conditionVal)
			{
				result.result = parseArithmetic(params[1], parsingContext, expression);
			}
			else
			{
				result.result = parseArithmetic(params[2], parsingContext, expression);
			}
		}
		else
		{
			unsigned numParams = ((params.size() == 1 && params[0] == "") ? 0 : params.size());

			FunctionSignature fnSignature = { fnString, numParams };

			ParsingContext::FunctionIteratorConst foundFunction = parsingContext.findFunction(fnSignature);

			if(foundFunction != parsingContext.endFunctionConst())
			{
				result.validFn = result.validDomain = true;

				ParsingContext newContext = ParsingContext(&parsingContext);

				for(unsigned i = 0; i < numParams; i++)
				{
					newContext.setVariable((*foundFunction).params[i], parseArithmetic(params[i], parsingContext, expression));
					// newMap[(foundFunction->second).params[i]] = parseArithmetic(params[i], parsingContext, expression);
				}

				result.result = parseArithmetic((*foundFunction).fnExpression, newContext, expression);
			}
		}
	}

	return result;
}

/* multimap<string, functionDefinition>::const_iterator findFunctionConst(const multimap<string, functionDefinition>& functionMap, string functionName, unsigned numParams)
{
	pair<multimap<string, functionDefinition>::const_iterator, multimap<string, functionDefinition>::const_iterator> matchingFunctionRange = functionMap.equal_range(functionName);

	multimap<string, functionDefinition>::const_iterator currentFunction = matchingFunctionRange.first, returnFunction;

	unsigned numFunctions = functionMap.count(functionName), i = 0;

	for(i = 0; i < numFunctions && (currentFunction->second).params.size() != numParams;)
	{
		i++;
		currentFunction++;
	}

	if(i < numFunctions)
	{
		returnFunction = currentFunction;
	}
	else
	{
		returnFunction = functionMap.end();
	}

	return returnFunction;
}

multimap<string, functionDefinition>::iterator findFunction(multimap<string, functionDefinition>& functionMap, string functionName, unsigned numParams)
{
	pair<multimap<string, functionDefinition>::iterator, multimap<string, functionDefinition>::iterator> matchingFunctionRange = functionMap.equal_range(functionName);

	multimap<string, functionDefinition>::iterator currentFunction = matchingFunctionRange.first, returnFunction;

	unsigned numFunctions = functionMap.count(functionName), i = 0;

	for(i = 0; i < numFunctions && (currentFunction->second).params.size() != numParams;)
	{
		i++;
		currentFunction++;
	}

	if(i < numFunctions)
	{
		returnFunction = currentFunction;
	}
	else
	{
		returnFunction = functionMap.end();
	}

	return returnFunction;
} */

string::size_type ExpressionParser::findClosingChar(char openStr, char closeStr, int openCharIndex, string expression)
{
	unsigned index = openCharIndex + 1;
	int charNestLevel = 1;

	while(charNestLevel > 0 && index < expression.length())
	{
		if(expression[index] == openStr)
		{
			charNestLevel++;
		}

		if(expression[index] == closeStr)
		{
			charNestLevel--;
		}

		if(charNestLevel > 0)
		{
			index++;
		}
	}

	if(charNestLevel > 0)
	{
		return string::npos;
	}
	else
	{
		return index;
	}
}

bool ExpressionParser::isVarChar(char ch)
{
	return isalpha(ch) || (ch == '_');
}

#ifdef MULTIPRECISION
calcFloat ExpressionParser::get_pi_value()
{
	return mpfr::const_pi();
}

calcFloat ExpressionParser::get_e_value()
{
	return mpfr::exp(calcFloat(1.0));
}
#else
calcFloat ExpressionParser::get_pi_value()
{
	return M_PI;
}

calcFloat ExpressionParser::get_e_value()
{
	return M_E;
}
#endif

calcFloat ExpressionParser::deriv(std::string expression,
			 std::string variable,
			 calcFloat value,
			 calcFloat delta,
			 const ParsingContext& parsingContext,
			 std::string parentExpression,
			 unsigned derivNum) const
{
	if(derivNum >= 0)
	{
		ParsingContext newContext = ParsingContext(&parsingContext);

		string typeErrorMsg = "Derivative type error";

		vector<calcFloat> diffValues;
		calcFloat startVal = value - (derivNum * delta / 2), currentVal = startVal, currentFnVal;

		for(unsigned i = 0; i < (derivNum + 1); i++)
		{
			newContext.setVariable(variable, currentVal);

			currentFnVal = parseArithmetic(expression, newContext, parentExpression).getVerifiedFloat(typeErrorMsg);

			diffValues.push_back(currentFnVal);

			currentVal += delta;
		}
		
		while(diffValues.size() > 1)
		{
			ParserMathematicalFunctions::deltaVector(diffValues);

			// cout << setprecision(50) << '{';

			for(unsigned i = 0; i < diffValues.size(); i++)
			{
				diffValues[i] /= delta;

				// cout << diffValues[i] << ' ';
			}

			// cout << '}' << endl;
		}

		return diffValues[0];
	}
	else
	{
		throw exception(("Invalid derivative number (" + to_string((long long)derivNum) + ")" + " specified in expression \"" + parentExpression + "\".").c_str());
	}


	//calcFloat leftVal, rightVal, derivative;

	//if(derivNum <= 1)
	//{
	//	newValueMap[variable] = value - delta;
	//	leftVal = parseArithmetic(expression, newValueMap, settings, resultHistory, parentExpression);

	//	//newValueMap[variable] = value;
	//	//middleVal = parseArithmetic(expression, newValueMap, settings, resultHistory, parentExpression);

	//	newValueMap[variable] = value + delta;
	//	rightVal = parseArithmetic(expression, newValueMap, settings, resultHistory, parentExpression);
	//}
	//else
	//{
	//	delta *= 10.0;

	//	leftVal = deriv(expression, variable, value - delta, delta, valueMap, settings, resultHistory, parentExpression, derivNum - 1);

	//	rightVal = deriv(expression, variable, value + delta, delta, valueMap, settings, resultHistory, parentExpression, derivNum - 1);

	//	// cout << "Derivative " << derivNum << ": Used " << leftVal << " and " << rightVal << " with delta " << delta << endl;
	//}

	//derivative = (rightVal - leftVal) / (2.0 * delta);

	//return derivative;
}

/*
calcFloat ExpressionParser::integral(std::string expression,
				std::string variable,
				calcFloat lower,
				calcFloat upper,
				calcFloat delta,
				const ParsingContext& parsingContext,
				std::string parentExpression)
{
	map<string, calcObj> newValueMap = valueMap;
	calcFloat total = 0.0, ptVal;

	string typeErrorMsg = "Integral type error";

	if(lower != upper)
	{
		for(calcFloat i = lower; i <= upper; i += delta)
		{
			newValueMap[variable] = i;
			ptVal = parseArithmetic(expression, newValueMap, functionMap, settings, resultHistory, parentExpression).getVerifiedFloat(typeErrorMsg);

			if(i == lower || (i + delta) > upper)
			{
				total += (ptVal * delta / 2);
			}
			else
			{
				total += (ptVal * delta);
			}
		}
	}

	return total;
} */

calcFloat ExpressionParser::gaussIntegral(std::string expression,
				std::string variable,
				calcFloat lower,
				calcFloat upper,
				unsigned estimationPoints,
				const ParsingContext& parsingContext,
				std::string parentExpression) const
{
	ParsingContext newContext = ParsingContext(&parsingContext);
	calcFloat total = 0.0, ptVal;

#ifdef MULTIPRECISION
	static vector<calcFloat> gaussNodes, gaussWeights;
#endif

	static unsigned cachedPrecision = 0, cachedEstimationPoints = 0;

	string typeErrorMsg = "Integral type error";

#ifdef MULTIPRECISION
//#ifdef MULTIPRECSISION
//	if(mpfr::mpreal::get_default_prec() != cachedPrecision || estimationPoints != cachedEstimationPoints)
//#else
	if (estimationPoints != cachedEstimationPoints)
//#endif
	{
		/*
		gaussNodes.push_back(calcFloat("0.9968934840746495402716300509186952833408820381178")); gaussWeights.push_back(calcFloat("0.0079681924961666056154658834746736224504806965872"));
		gaussNodes.push_back(calcFloat("0.9836681232797472099700325816056628019403178547097")); gaussWeights.push_back(calcFloat("0.01846646831109095914230213191204726909620653396818"));
		gaussNodes.push_back(calcFloat("0.9600218649683075122168710255817976629303592174039")); gaussWeights.push_back(calcFloat("0.0287847078833233693497191796112920436395888945463"));
		gaussNodes.push_back(calcFloat("0.9262000474292743258793242770804740040864745368253")); gaussWeights.push_back(calcFloat("0.0387991925696270495968019364463476920332009767664"));
		gaussNodes.push_back(calcFloat("0.8825605357920526815431164625302255900566891471465")); gaussWeights.push_back(calcFloat("0.04840267283059405290293814042280751781527180919737"));
		gaussNodes.push_back(calcFloat("0.8295657623827683974428981197325019164390686961703")); gaussWeights.push_back(calcFloat("0.0574931562176190664817216894020561287971206707218"));
		gaussNodes.push_back(calcFloat("0.7677774321048261949179773409745031316948836172329")); gaussWeights.push_back(calcFloat("0.0659742298821804951281285151159623612374429536567"));
		gaussNodes.push_back(calcFloat("0.697850494793315796932292388026640068382353800654")); gaussWeights.push_back(calcFloat("0.07375597473770520626824385002219073415377052603705"));
		gaussNodes.push_back(calcFloat("0.6205261829892428611404775564311892992073646928295")); gaussWeights.push_back(calcFloat("0.0807558952294202153546949384605297308758928037084"));
		gaussNodes.push_back(calcFloat("0.5366241481420198992641697933110727941641780069303")); gaussWeights.push_back(calcFloat("0.0868997872010829798023875307151257025767533287435"));
		gaussNodes.push_back(calcFloat("0.44703376953808917678060990032285400016240759386142")); gaussWeights.push_back(calcFloat("0.0921225222377861287176327070876187671969132344182"));
		gaussNodes.push_back(calcFloat("0.3527047255308781134710372070893738606536310080214")); gaussWeights.push_back(calcFloat("0.09636873717464425963946862635180986509640646143016"));
		gaussNodes.push_back(calcFloat("0.25463692616788984643980512981780510788278930330252")); gaussWeights.push_back(calcFloat("0.0995934205867952670627802821035694765298692636667"));
		gaussNodes.push_back(calcFloat("0.15386991360858354696379467274325592041855197124434")); gaussWeights.push_back(calcFloat("0.1017623897484055045964289521685540446327062894871"));
		gaussNodes.push_back(calcFloat("0.0514718425553176958330252131667225737491414536666")); gaussWeights.push_back(calcFloat("0.1028526528935588403412856367054150438683755570649"));
		

		const unsigned vecSize = gaussNodes.size();

		for(unsigned i = 0; i < vecSize; i++)
		{
			gaussNodes.push_back(-gaussNodes[i]);
			gaussWeights.push_back(gaussWeights[i]);
		}
		*/
//#ifdef MULTIPRECSISION
		cachedPrecision = mpfr::mpreal::get_default_prec();
//#endif
		cachedEstimationPoints = estimationPoints;

		gaussNodes.clear();
		gaussWeights.clear();

		LegendrePolynomial nodeFindingPolynomial(estimationPoints), weightDenominatorPolynomial(estimationPoints - 1);

		ParserMathematicalFunctions::findMultipleBisectionSolutions(&nodeFindingPolynomial, 0.0, 1.0, gaussNodes, estimationPoints * estimationPoints);

		unsigned originalNodeResultsSize = gaussNodes.size();

		for (unsigned i = 0; i < originalNodeResultsSize; i++)
		{
//#ifdef MULTIPRECSISION
			if (i >= 1 || (!mpfr::iszero(gaussNodes[i]) && gaussNodes[i].get_exp() >= -(gaussNodes[i].getPrecision())))
//#else
//			if(i >= 1 || gaussNodes[i] != 0.0)
//#endif
			{
				gaussNodes.push_back(-gaussNodes[i]);
			}
		}

		gaussWeights.resize(gaussNodes.size());

		for (unsigned i = 0; i < gaussNodes.size(); i++)
		{
//#ifdef MULTIPRECISION
			gaussWeights[i] = mpfr::mul_2ui(1.0 - mpfr::sqr(gaussNodes[i]), 1)
				/ mpfr::sqr(estimationPoints * weightDenominatorPolynomial.evaluate(gaussNodes[i]));
//#else
//			calcFloat denominatorValue = estimationPoints * weightDenominatorPolynomial.evaluate(gaussNodes[i]);
//			gaussWeights[i] = (2 * (1.0 - gaussNodes[i] * gaussNodes[i]))
//							 / (denominatorValue * denominatorValue);
//#endif
		}

		
		// cout << setprecision(mpfr::bits2digits(mpfr::mpreal::get_default_prec()));

		/*
		for (unsigned i = 0; i < gaussNodes.size(); i++)
		{
			cout << gaussNodes[i] << ' ' << gaussWeights[i] << endl;
		}
		*/

		//for(int i = 0; i < gaussNodes.size(); i++)
		//{
		//	cout << gaussNodes[i] << ' ' << gaussWeights[i] << endl;
		//}

	}
#endif


	if(lower != upper)
	{
		for(size_t i = 0; i < gaussNodes.size(); i++)
		{
			newContext.setVariable(variable, scaleNum(-1.0, 1.0, lower, upper, gaussNodes[i]));
			ptVal = parseArithmetic(expression, newContext, parentExpression).getVerifiedFloat(typeErrorMsg);

			total += (ptVal * gaussWeights[i]);
		}
	}

	total *= (upper - lower) / 2.0;

	return total;
}

calcFloat ExpressionParser::scaleNum(calcFloat oldMin, calcFloat oldMax, calcFloat newMin, calcFloat newMax, calcFloat value)
{
	calcFloat slope = (newMax - newMin) / (oldMax - oldMin);
	calcFloat xIntercept = newMin - (oldMin * slope);

	return (value * slope) + xIntercept;
}

void ExpressionParser::evalSeq(std::vector<calcObj>& result,
			 std::string expression,
			 std::string variable,
			 calcFloat lower,
			 calcFloat upper,
			 const ParsingContext& parsingContext,
			 std::string parentExpression,
			 bool automaticallyNegateDelta,
			 calcFloat delta) const
{
	ParsingContext newContext = ParsingContext(&parsingContext);
	calcObj thisPt;

	// cout << '{';

	if(delta == 0.0)
	{
		throw DomainError("parsing of sequence", expression, "delta cannot be zero");
	}
	else
	{
		unsigned resultSize;
				 // thisIndex = 0;

		if (automaticallyNegateDelta && delta > 0.0 && (lower > upper))
		{
			delta = -delta;
		}

		if (!((upper > lower && delta > 0.0) || (upper < lower && delta < 0.0) || (upper == lower)))
		{
			throw DomainError("parsing of sequence", expression, "invalid delta sign");
		}
		else
		{
			try
			{
				resultSize = floatToUnsigned(floor(abs((upper - lower) / delta))) + 1;
			}
			catch (NumericConversionError ex)
			{
				throw DomainError("parsing of sequence", expression, "invalid range");
			}

			result.resize(resultSize);

			// for (calcFloat thisVal = lower; (delta > 0) ? (thisVal <= upper) : (thisVal >= upper); thisVal += delta)
			for(unsigned thisIndex = 0; thisIndex < resultSize; thisIndex++)
			{
				calcFloat thisVal = lower + (thisIndex * delta);

				newContext.setVariable(variable, thisVal);
				result[thisIndex] = parseArithmetic(expression, newContext, parentExpression);

				// thisIndex++;
				// cout << thisVal << ':' << thisPt << ' ';
			}
		}
	}

	// cout << '}' << endl;
}

ExpressionParser::SingleVariableParsedFunction::SingleVariableParsedFunction(string expression, string parentExpression, string variable, string typeErrorStr,
												const ExpressionParser* parser, const ExpressionParser::ParsingContext* parsingContext):
	expression(expression),
	parentExpression(parentExpression),
	variable(variable),
	typeErrorStr(typeErrorStr),
	parser(parser),
	parsingContext(parsingContext)
{
}

calcFloat ExpressionParser::SingleVariableParsedFunction::evaluate(calcFloat value) const
{
	ParsingContext newContext = ParsingContext(this->parsingContext);
	newContext.setVariable(this->variable, value);

	return (this->parser->parseArithmetic(this->expression, newContext, this->parentExpression).getVerifiedFloat(this->typeErrorStr));
}

ExpressionParser::LegendrePolynomial::LegendrePolynomial(unsigned polynomialOrder):
	polynomialOrder(polynomialOrder)
{
	this->summationFactors.resize(this->polynomialOrder + 1);
	long maxSummationFactorPrecision = 0;

	/*
	cout << 'P' << polynomialOrder << ':' << endl;
	cout << setprecision(mpfr::bits2digits(mpfr::mpreal::get_default_prec()));
	*/

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

		// cout << integerResult << " -> " << this->summationFactors[i] << endl;
	}

#ifdef MULTIPRECISION
	this->calculationPrecision = mpfr::mpreal::get_default_prec() + maxSummationFactorPrecision; // + mpfr::digits2bits((long)(log10(this->polynomialOrder) + 1));
#endif
}

calcFloat ExpressionParser::LegendrePolynomial::evaluate(calcFloat value) const
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
	// mpfr::mpreal::set_default_prec(originalPrecision);
	sumValue.setPrecision(mpfr::mpreal::get_default_prec());
#endif

	return sumValue;
}

void ExpressionParser::bisectionSolve(std::string expression,
					std::string variable,
					calcFloat lBound,
					calcFloat hBound,
					std::vector<calcFloat>& solutions,
					unsigned numIntervals,
					const ParsingContext& parsingContext,
					std::string parentExpression) const
{
	// map<string, calcObj> newValueMap = valueMap;
	// calcFloat lBoundVal, average, averageVal, hBoundVal;

	string typeErrorStr = "Equation solver type error";

	SingleVariableParsedFunction functionEvaluator = SingleVariableParsedFunction(expression, parentExpression, variable, typeErrorStr, this, &parsingContext);

	findMultipleBisectionSolutions(&functionEvaluator, lBound, hBound, solutions, numIntervals);
	/* average = (lBound + hBound) / 2;

	if(usingCachedValues)
	{
		lBoundVal = cachedLower;
		hBoundVal = cachedUpper;
	}
	else
	{
		newValueMap[variable] = lBound;
		lBoundVal = parseArithmetic(expression, newValueMap, functionMap, settings, resultHistory, parentExpression).getVerifiedFloat(typeErrorStr);

		newValueMap[variable] = hBound;
		hBoundVal = parseArithmetic(expression, newValueMap, functionMap, settings, resultHistory, parentExpression).getVerifiedFloat(typeErrorStr);
	}

	newValueMap[variable] = average;
	averageVal = parseArithmetic(expression, newValueMap, functionMap, settings, resultHistory, parentExpression).getVerifiedFloat(typeErrorStr);

	// cout << lBound << ' ' << lBoundVal << ';' << average << ' ' << averageVal << ';' << hBound << ' ' << hBoundVal << endl;

	if(lBoundVal == 0.0 && find(solutions.begin(), solutions.end(), lBound) == solutions.end())
	{
		solutions.push_back(lBound);
	}
	
	if(hBoundVal == 0.0 && find(solutions.begin(), solutions.end(), hBound) == solutions.end())
	{
		solutions.push_back(hBound);
	}

	if(averageVal == 0.0 && find(solutions.begin(), solutions.end(), average) == solutions.end())
	{
		solutions.push_back(average);
	}

	if(currentRecursion < maxRecursions)
	{
		if(lBoundVal * averageVal < 0.0)
		{
			bisectionSolve(expression, variable, lBound, average, solutions, valueMap, functionMap, settings, resultHistory, parentExpression, maxRecursions, currentRecursion + 1, true, lBoundVal, averageVal);
		}

		if(averageVal * hBoundVal < 0.0)
		{
			bisectionSolve(expression, variable, average, hBound, solutions, valueMap, functionMap, settings, resultHistory, parentExpression, maxRecursions, currentRecursion + 1, true, averageVal, hBoundVal);
		}
	}
	else
	{
		solutions.push_back(average);
	}

	if(currentRecursion == 0)
	{
		sort(solutions.begin(), solutions.end());
	} */
}

//void primeFactorization(vector<calcFloat>& results, calcFloat number)
//{
//	bool factorFound;
//
//	do
//	{
//		factorFound = false;
//
//		calcFloat squareRoot = sqrt(number);
//
//		for(calcFloat factor = 2; !factorFound && factor <= squareRoot; factor++)
//		{
//			calcFloat remainder = fmod(number, factor);
//
//			if(remainder < 0.1 || abs(remainder - factor) < 0.1)
//			{
//				results.push_back(factor);
//
//				number /= factor;
//
//				cout << "Number: " << number << endl;
//
//				factorFound = true;
//			}
//		}
//	} while(factorFound);
//
//	results.push_back(number);
//}