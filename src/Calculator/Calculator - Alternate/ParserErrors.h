#pragma once

#include <string>

class ExpressionError : public std::exception
{
protected:
	std::string errorMessage,
		expression,
		outputStr;
public:
	ExpressionError(const std::string& message = "Unknown error", const std::string& expression = "", bool genOutputStr = true)
	{
		this->errorMessage = message;
		this->expression = expression;

		if (genOutputStr)
		{
			this->generateOutputStr();
		}
	}

	virtual void generateOutputStr()
	{
		if (this->expression != "")
		{
			this->outputStr = (this->errorMessage + " in expression \"" + this->expression + "\".");
		}
		else
		{
			this->outputStr = (this->errorMessage + " in expression.");
		}
	}

	std::string getExpression() const
	{
		return this->expression;
	}

	void setExpression(std::string newExpression)
	{
		this->expression = newExpression;
		this->generateOutputStr();
	}

	virtual const char* what() const throw()
	{
		return outputStr.c_str();
	}
};

class SyntaxError : public ExpressionError
{
public:
	SyntaxError(const std::string& expression) :
		ExpressionError("Syntax error", expression)
	{

	}
};

class DomainError : public ExpressionError
{
private:
	std::string expressionDescription,
		specificInformation;
public:
	DomainError(std::string expressionDescription, std::string expression = "", std::string specificInformation = "") :
		ExpressionError("Domain error", expression, false),
		expressionDescription(expressionDescription),
		specificInformation(specificInformation)
	{
		this->expressionDescription = expressionDescription;
		this->generateOutputStr();
	}

	std::string getExpressionDescription() const
	{
		return this->expressionDescription;
	}

	void setExpressionDescription(std::string newDescription)
	{
		this->expressionDescription = newDescription;
		this->generateOutputStr();
	}

	std::string getSpecificInformation() const
	{
		return this->specificInformation;
	}

	void setSpecificInformation(std::string newInformation)
	{
		this->specificInformation = newInformation;
		this->generateOutputStr();
	}

	virtual void generateOutputStr()
	{
		this->outputStr = this->errorMessage;

		if (this->specificInformation != "")
		{
			this->outputStr += (" (" + this->specificInformation + ")");
		}

		this->outputStr += (" in " + this->expressionDescription);

		if (this->expression != "")
		{
			this->outputStr += (" \"" + this->expression + "\".");
		}
	}

	/* virtual const char* what() const throw()
	{
	std::string returnStr = (this->errorMessage + " in " + this->expressionDescription);

	if(this->expression != "")
	{
	returnStr += (" \"" + this->expression + "\".");
	}

	return returnStr.c_str();
	} */
};

class OverflowError : public ExpressionError
{
public:
	OverflowError(const std::string& expression) :
		ExpressionError("Overflow error", expression)
	{

	}
};

class ExpressionTypeError : public ExpressionError
{
public:
	ExpressionTypeError(const std::string& expression, const std::string typeErrorDescription = "Type error") :
		ExpressionError(typeErrorDescription, expression)
	{

	}
};

class ExpressionDivisionByZeroError : public ExpressionError
{
public:
	ExpressionDivisionByZeroError(const std::string& expression) :
		ExpressionError("Division by zero error", expression)
	{

	}
};