#pragma once
#include <string>

struct FunctionSignature
{
	std::string functionName;
	unsigned numParams;

	bool operator<(const FunctionSignature & otherSignature) const
	{
		return (this->functionName < otherSignature.functionName)
			|| (this->functionName == otherSignature.functionName && this->numParams < otherSignature.numParams);
	}

	bool operator<=(const FunctionSignature & otherSignature) const
	{
		return (*this < otherSignature || *this == otherSignature);
	}

	bool operator>(const FunctionSignature & otherSignature) const
	{
		return !(*this <= otherSignature);
	}

	bool operator>=(const FunctionSignature & otherSignature) const
	{
		return !(*this < otherSignature);
	}

	bool operator==(const FunctionSignature & otherSignature) const
	{
		return (this->functionName == otherSignature.functionName)
			&& (this->numParams == otherSignature.numParams);
	}

	bool operator!=(const FunctionSignature & otherSignature) const
	{
		return !(*this == otherSignature);
	}
};
