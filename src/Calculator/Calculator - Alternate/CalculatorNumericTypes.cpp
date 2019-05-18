#include "CalculatorNumericTypes.h"

using namespace std;

#ifdef MULTIPRECISION
void floatToInt(const calcFloat& floatValue, calcSignedInt& destination)
{
	mpz_t gmpInt;
	mpz_init2(gmpInt, floatValue.getPrecision());

	mpfr_clear_erangeflag();
	mpfr_clear_inexflag();

	mpfr_get_z(gmpInt, floatValue.mpfr_srcptr(), mpfr_rnd_t::MPFR_RNDZ);

	if (mpfr_erangeflag_p() || mpfr_inexflag_p())
	{
		throw NumericConversionError();
	}
	else
	{
		destination = gmpInt;
	}
}

unsigned floatToUnsigned(const calcFloat& floatValue)
{
	unsigned long result;
	mpfr_clear_erangeflag();
	mpfr_clear_inexflag();

	result = floatValue.toULong();

	if (mpfr_erangeflag_p() || mpfr_inexflag_p() || result > numeric_limits<unsigned>::max())
	{
		throw NumericConversionError();
	}
	else
	{
		return (unsigned)result;
	}
}

void intToFloat(const calcSignedInt& intValue, calcFloat& destination)
{
	mpfr_t mpfrFloat;

	mpfr_clear_erangeflag();
	mpfr_clear_inexflag();

	mpfr_init2(mpfrFloat, mpfr::mpreal::get_default_prec()); // max(mpz_sizeinbase(intValue.backend().data(), 2), (size_t)2));
	mpfr_set_z(mpfrFloat, intValue.backend().data(), mpfr_rnd_t::MPFR_RNDZ);

	if (mpfr_erangeflag_p() || mpfr_inexflag_p())
	{
		throw NumericConversionError();
	}
	else
	{
		destination = mpfr::mpreal(mpfrFloat);
	}
}

void stringToInt(const std::string& stringValue, calcSignedInt& destination)
{
	int conversionSuccess = mpz_set_str(destination.backend().data(), stringValue.c_str(), 10);

	if (conversionSuccess != 0)
	{
		throw NumericConversionError();
	}
}

void stringToFloat(const std::string& stringValue, calcFloat& destination)
{
	int conversionSuccess = mpfr_set_str(destination.mpfr_ptr(), stringValue.c_str(), 10, mpfr_get_default_rounding_mode());
	
	if (conversionSuccess != 0)
	{
		throw NumericConversionError();
	}
}
#else
void floatToInt(const calcFloat& floatValue, calcSignedInt& destination)
{
	if (floatValue >= numeric_limits<calcSignedInt>::min() && floatValue <= numeric_limits<calcSignedInt>::max()
		&& rint(floatValue) == floatValue)
	{
		destination = static_cast<calcSignedInt>(floatValue);
	}
	else
	{
		throw NumericConversionError();
	}
}

unsigned floatToUnsigned(const calcFloat& floatValue)
{
	if (floatValue >= numeric_limits<unsigned>::min() && floatValue <= numeric_limits<unsigned>::max()
		&& rint(floatValue) == floatValue)
	{
		return static_cast<unsigned>(floatValue);
	}
	else
	{
		throw NumericConversionError();
	}
}

void intToFloat(const calcSignedInt& intValue, calcFloat& destination)
{
	if (abs(intValue) <= numeric_limits<calcFloat>::max())
	{
		destination = static_cast<calcFloat>(intValue);
	}
	else
	{
		throw NumericConversionError();
	}
}

void stringToInt(const std::string& stringValue, calcSignedInt& destination)
{
	errno = 0;

	char* lastCharPtr = NULL;
	long long resultValue = strtoll(stringValue.c_str(), &lastCharPtr, 10);

	if (errno == 0 && lastCharPtr == stringValue.c_str() + stringValue.length())
	{
		destination = resultValue;
	}
	else
	{
		throw NumericConversionError();
	}
}

void stringToFloat(const std::string& stringValue, calcFloat& destination)
{
	char* parseEndPtr = NULL;

	errno = 0;

	double resultValue = strtod(stringValue.c_str(), &parseEndPtr);

	if (errno == 0 && parseEndPtr == stringValue.c_str() + stringValue.length())
	{
		destination = resultValue;
	}
	else
	{
		throw NumericConversionError();
	}
}
#endif

unsigned stringToUnsigned(const string& stringValue)
{
	errno = 0;

	char* lastCharPtr = NULL;
	unsigned long resultValue = strtoul(stringValue.c_str(), &lastCharPtr, 10);

	if (errno == 0 && lastCharPtr == stringValue.c_str() + stringValue.length()
		&& resultValue >= numeric_limits<unsigned>::min() && resultValue <= numeric_limits<unsigned>::max())
	{
		return static_cast<unsigned>(resultValue);
	}
	else
	{
		throw NumericConversionError();
	}
}