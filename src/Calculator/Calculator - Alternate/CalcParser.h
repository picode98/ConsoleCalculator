#pragma once

#include "ParserCore.h"
#include "BuiltInFunctions.h"

class CalcParser : public ExpressionParser
{
public:
	CalcParser()
	{
		this->builtInFunctions = getAllBuiltinFunctions();
	}
};