#pragma once

#ifndef CALC_OBJ_H
#define CALC_OBJ_H

// #define MULTIPRECISION

/* #include <boost/math/constants/constants.hpp>

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/math/special_functions/gamma.hpp> */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <boost/math/special_functions/next.hpp>

#include "GlobalBuildVariables.h"
#include "CalculatorNumericTypes.h"
#include "StringUtils.h"

/* using boost::multiprecision::cpp_dec_float_50;
using namespace boost::multiprecision;

typedef cpp_dec_float_50 calcFloat;
typedef number< cpp_int_backend<0, 0, signed_magnitude> > calcSignedInt;
typedef number< cpp_int_backend<128, 128, unsigned_magnitude> > calcUnsignedInt; */

enum calcObjType{TYPE_FLOAT, TYPE_LIST};
enum decimalFractionMode {DECIMAL_VALUE, FRACTION_VALUE};

unsigned significantFigures(const std::string& numStr);

class type_error : public std::exception
{
private:
	std::string errorMessage;
public:
	type_error(const std::string& message)
	{
		this->errorMessage = message;
	}

	virtual const char* what() const throw()
	{
		return (this->errorMessage).c_str();
	}
};

class DivisionByZeroError : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Division by zero error";
	}
};

class Fraction
{
public:
	calcSignedInt numerator,
				  denominator;

	friend std::ostream& operator<<(std::ostream& outputStream, Fraction value);
};

class calcObj
{
public:
	struct StreamSettings
	{
		bool CSVOutputMode = false,
			 fullPrecision = false;

		decimalFractionMode decFracMode = DECIMAL_VALUE;
	};
private:
	calcObjType type;
	calcFloat floatObj;
	std::vector<calcFloat> listObj;
	void resetObjects();
	// static void split(const std::string& s, char delim, std::vector<std::string>& elems);

	/*
	static int getCSVFormatIndex();
	static int getDecimalFractionFormatIndex();
	static int getFullPrecisionFormatIndex();
	*/

	static int getStreamSettingsFormatIndex();

	static void streamSettingsDestructorCallback(std::ios_base::event eventInfo, std::ios_base& stream, int settingsIndex);

	static StreamSettings* getStreamSettings(std::ostream& outputStream, bool createSettings);
public:
	static StreamSettings getStreamSettings(std::ostream& outputStream);

	static std::ostream& CSVFormat(std::ostream& outputStream);
	static std::ostream& nonCSVFormat(std::ostream& outputStream);
	static std::ostream& decimalFormat(std::ostream& outputStream);
	static std::ostream& fractionFormat(std::ostream& outputStream);
	static std::ostream& setFullPrecision(std::ostream& outputStream);
	static std::ostream& unsetFullPrecision(std::ostream& outputStream);

	/*
	static bool getCSVOutputMode(std::ostream& outputStream);
	static decimalFractionMode getDecimalFractionOutputMode(std::ostream& outputStream);
	static bool getFullPrecision(std::ostream& outputStream);
	*/

	calcObj();
	calcObj(const std::string& objString, bool preservePrecision);
	calcObj(const calcFloat& objFloat);
#ifdef MULTIPRECISION
	calcObj(const int& objInt);
#endif
	calcObj operator+(const calcObj& addObj);
	calcObj operator-(const calcObj& subObj);
	calcObj operator*(const calcObj& multObj);
	calcObj operator/(const calcObj& divObj);
	calcObj operator+=(const calcObj& addObj);
	calcObj operator-=(const calcObj& subObj);
	calcObj operator*=(const calcObj& multObj);
	calcObj operator/=(const calcObj& divObj);
	calcObj operator=(const calcObj& assignObj);
	calcObj operator=(const calcFloat& assignObj);
	calcObj operator-();
	bool operator==(const calcObj& compareObj);
	bool operator!=(const calcObj& compareObj);
	bool operator<(const calcObj& compareObj);
	bool operator>(const calcObj& compareObj);
	bool operator<=(const calcObj& compareObj);
	bool operator>=(const calcObj& compareObj);

	calcFloat getVerifiedFloat(std::string typeErrorMsg);
	void set_list(const std::vector<calcFloat>& assignObj);
	bool get_list(std::vector<calcFloat>& buffer);
	calcObjType get_type();
	calcFloat get_list_index(unsigned index);
	void set_list_index(unsigned index, calcFloat value);
	unsigned get_list_length();
	void concat_list(const calcObj& concatObj);
	void sublist(std::vector<calcFloat>& buffer, unsigned startIndex, unsigned endIndex);

#ifdef MULTIPRECISION
	calcObj& set_precision(long newPrecision);
#endif

	std::string calcObj::getString(StreamSettings settings, unsigned precision) const;
	// void getString(std::string& destination, decimalFractionMode decimalFractionFormat, bool csvMode);

	friend std::ostream& operator<<(std::ostream& out, const calcObj& outObj);
	friend calcObj pow(const calcObj& base, const calcObj& power);
	friend calcObj fmod(const calcObj& base, const calcObj& power);
};

bool floatToFraction(Fraction* result, calcFloat value, calcSignedInt maximumDenominator, calcFloat maximumErrorMultiple);
void toVerifiedFloatVector(std::vector<calcObj>& calcObjVec, std::vector<calcFloat>& calcFloatVec, std::string typeErrorMsg);

#endif