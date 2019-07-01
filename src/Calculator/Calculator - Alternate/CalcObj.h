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

class CalcObj
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

	CalcObj();
	CalcObj(const std::string& objString, bool preservePrecision);
	CalcObj(const calcFloat& objFloat);
#ifdef MULTIPRECISION
	CalcObj(const int& objInt);
#endif
	CalcObj operator+(const CalcObj& addObj);
	CalcObj operator-(const CalcObj& subObj);
	CalcObj operator*(const CalcObj& multObj);
	CalcObj operator/(const CalcObj& divObj);
	CalcObj operator+=(const CalcObj& addObj);
	CalcObj operator-=(const CalcObj& subObj);
	CalcObj operator*=(const CalcObj& multObj);
	CalcObj operator/=(const CalcObj& divObj);
	CalcObj operator=(const CalcObj& assignObj);
	CalcObj operator=(const calcFloat& assignObj);
	CalcObj operator-();
	bool operator==(const CalcObj& compareObj);
	bool operator!=(const CalcObj& compareObj);
	bool operator<(const CalcObj& compareObj);
	bool operator>(const CalcObj& compareObj);
	bool operator<=(const CalcObj& compareObj);
	bool operator>=(const CalcObj& compareObj);

	calcFloat getVerifiedFloat(std::string typeErrorMsg);
	void set_list(const std::vector<calcFloat>& assignObj);
	bool get_list(std::vector<calcFloat>& buffer);
	calcObjType get_type();
	calcFloat get_list_index(unsigned index);
	void set_list_index(unsigned index, calcFloat value);
	unsigned get_list_length();
	void concat_list(const CalcObj& concatObj);
	void sublist(std::vector<calcFloat>& buffer, unsigned startIndex, unsigned endIndex);

#ifdef MULTIPRECISION
	CalcObj& set_precision(long newPrecision);
#endif

	std::string CalcObj::getString(StreamSettings settings, unsigned precision) const;
	// void getString(std::string& destination, decimalFractionMode decimalFractionFormat, bool csvMode);

	friend std::ostream& operator<<(std::ostream& out, const CalcObj& outObj);
	friend CalcObj pow(const CalcObj& base, const CalcObj& power);
	friend CalcObj fmod(const CalcObj& base, const CalcObj& power);
};

bool floatToFraction(Fraction* result, calcFloat value, calcSignedInt maximumDenominator, calcFloat maximumErrorMultiple);
void toVerifiedFloatVector(std::vector<CalcObj>& calcObjVec, std::vector<calcFloat>& calcFloatVec, std::string typeErrorMsg);

#endif