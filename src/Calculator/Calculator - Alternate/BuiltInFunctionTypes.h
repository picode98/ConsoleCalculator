#pragma once
#include <map>
#include <variant>

#include "CalcObj.h"
#include "ParserErrors.h"

class ExpressionParser;
class ParsingContext_Export;


class BuiltInFunctionDefinition
{
public:
	struct ParameterSpecs
	{
		std::vector<bool> parsedArguments;
		std::set<unsigned> acceptedArgs;
		bool acceptsExtraArgs,
			parseExtraArgs = true;

		ParameterSpecs(const std::vector<bool>& parsedArguments,
			const std::set<unsigned>& acceptedArgs,
			bool acceptsExtraArgs,
			bool parseExtraArgs = false
			):
			parsedArguments(parsedArguments),
			acceptedArgs(acceptedArgs),
			acceptsExtraArgs(acceptsExtraArgs),
			parseExtraArgs(parseExtraArgs)
		{
		
		}

		ParameterSpecs() : ParameterSpecs(std::vector<bool>(), std::set<unsigned>(), false)
		{
			
		}
	};

	virtual CalcObj evaluate(const ExpressionParser* parser, const ParsingContext_Export& parsingContext, const std::vector<std::variant<std::string, CalcObj>>& arguments, const
	                         std::string& fnName, const
	                         std::string& fnExpression) = 0;
	virtual ParameterSpecs getParameterSpecs() = 0;

	virtual ~BuiltInFunctionDefinition() = default;
};

/*struct BuiltInFunctionOverloadGroup
{
	std::map<unsigned, BuiltInFunctionDefinition&> overloadList;

	BuiltInFunctionDefinition* extraParamsOverload;
};*/

class BuiltInFunctionDomainError : public DomainError
{
public:
	BuiltInFunctionDomainError(const std::string& specificInformation) :
		DomainError("function call", "", specificInformation)
	{

	}
};