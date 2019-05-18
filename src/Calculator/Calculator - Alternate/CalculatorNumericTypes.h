#pragma once

#include <string>
#include <limits>
#include <cmath>
#include <errno.h>

#include "GlobalBuildVariables.h"

#ifdef MULTIPRECISION
#include "mpreal.h"
typedef mpfr::mpreal calcFloat;

#include <boost\multiprecision\gmp.hpp>
typedef boost::multiprecision::mpz_int calcSignedInt;
#else
typedef double calcFloat;
typedef long long calcSignedInt;
#endif

void floatToInt(const calcFloat& floatValue, calcSignedInt& destination);
unsigned floatToUnsigned(const calcFloat& floatValue);
void intToFloat(const calcSignedInt& intValue, calcFloat& destination);
unsigned stringToUnsigned(const std::string& stringValue);
void stringToInt(const std::string& stringValue, calcSignedInt& destination);
void stringToFloat(const std::string& stringValue, calcFloat& destination);

inline calcFloat boolToFloat(bool val)
{
	return (val ? 1.0 : 0.0);
}

class NumericConversionError : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Numeric conversion error";
	}
};
