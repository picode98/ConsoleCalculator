#include <string>

#include "CalcObj.h"

using namespace std;

unsigned significantFigures(const string& numStr)
{
	unsigned resultValue;
	unsigned ePosition = numStr.find('E'); // string::npos;

	/* for (unsigned i = 0; i < numStr.size() && ePosition == string::npos; i++)
	{
		if (tolower(numStr[i]) == 'e')
		{
			ePosition = i;
		}
	} */

	if (ePosition != string::npos)
	{
		string preEStr = numStr.substr(0, ePosition);

		resultValue = significantFigures(preEStr);
	}
	else
	{
		unsigned startPosition, endPosition;
		bool foundStart = false, foundEnd = false;

		for (unsigned i = 0; i < numStr.size() && !foundStart; i++)
		{
			if (isdigit(numStr[i]) && numStr[i] != '0')
			{
				startPosition = i;
				foundStart = true;
			}
		}

		unsigned decimalPointPosition = numStr.find('.');
		if (decimalPointPosition != string::npos)
		{
			if (foundStart)
			{
				if (decimalPointPosition > startPosition)
				{
					resultValue = (numStr.size() - startPosition) - 1;
				}
				else
				{
					resultValue = (numStr.size() - startPosition);
				}
			}
			else
			{
				resultValue = (numStr.size() - 1) - decimalPointPosition;
			}
		}
		else
		{
			if (foundStart)
			{
				for (int i = (numStr.size() - 1); i >= 0 && !foundEnd; i--)
				{
					if (isdigit(numStr[i]) && numStr[i] != '0')
					{
						endPosition = i;
						foundEnd = true;
					}
				}

				resultValue = endPosition - startPosition + 1;
			}
			else
			{
				resultValue = 0;
			}
		}
	}

	return resultValue;
}

bool floatToFraction(Fraction* result, calcFloat value, calcSignedInt maximumDenominator, calcFloat maximumErrorMultiple)
{
	bool successfulConversion = false;
	// fraction resultValue;
	calcFloat integerPart, fractionalPart = value, numerator1 = 1, numerator2 = 0,
			  denominator1 = 0, denominator2 = 1, floatMaximumDenominator;

	intToFloat(maximumDenominator, floatMaximumDenominator);

#ifdef MULTIPRECISION
	calcFloat maximumError = mpfr::machine_epsilon(value) * maximumErrorMultiple;
#endif

	/* for(calcSignedInt thisDenominator = 1; (thisDenominator < maximumDenominator) && !successfulConversion; thisDenominator++)
	{
		calcFloat floatDenominator;
		intToFloat(thisDenominator, floatDenominator);

		calcFloat floatNumerator = value * floatDenominator;
		calcFloat distanceFromInteger = abs(floatNumerator - round(floatNumerator)) / floatDenominator;

		if(distanceFromInteger <= maximumError)
		{
			result->denominator = thisDenominator;

			floatToInt(round(floatNumerator), result->numerator);

			successfulConversion = true;
		}
	} */

	do
	{
		calcFloat tempNumerator,
			tempDenominator;

#ifdef MULTIPRECISION
		mpfr_trunc(integerPart.mpfr_ptr(), fractionalPart.mpfr_srcptr());
		mpfr_frac(fractionalPart.mpfr_ptr(), fractionalPart.mpfr_srcptr(), mpfr::mpreal::get_default_rnd());
#else
		integerPart = trunc(fractionalPart);
		fractionalPart -= integerPart;
#endif

		fractionalPart = (1.0 / fractionalPart);

		tempNumerator = (integerPart * numerator1) + numerator2;
		// cout << tempNumerator << " = " << integerPart << " * " << numerator1 << " + " << numerator2 << endl;

		tempDenominator = (integerPart * denominator1) + denominator2;
		// cout << tempDenominator << " = " << integerPart << " * " << denominator1 << " + " << denominator2 << endl;

		numerator2 = numerator1;
		denominator2 = denominator1;
		numerator1 = tempNumerator;
		denominator1 = tempDenominator;
#ifdef MULTIPRECISION
	} while (abs(numerator1 / denominator1 - value) > maximumError && denominator1 <= floatMaximumDenominator);
#else
	} while (abs(boost::math::float_distance(numerator1 / denominator1, value)) > maximumErrorMultiple && denominator1 <= floatMaximumDenominator);
#endif
	
	if (denominator1 <= floatMaximumDenominator)
	{
		successfulConversion = true;
		floatToInt(numerator1, result->numerator);
		floatToInt(denominator1, result->denominator);
	}

	return successfulConversion;
}

void toVerifiedFloatVector(vector<CalcObj>& calcObjVec, vector<calcFloat>& calcFloatVec, string typeErrorMsg)
{
	calcFloatVec.resize(calcObjVec.size());

	for(unsigned i = 0; i < calcObjVec.size(); i++)
	{
		calcFloatVec[i] = calcObjVec[i].getVerifiedFloat(typeErrorMsg);
	}
}

std::ostream& operator<<(std::ostream& outputStream, Fraction value)
{
#ifdef MULTIPRECISION
	if ((value.numerator.sign() < 0) != (value.denominator.sign() < 0))
#else
	if ((value.numerator < 0) != (value.denominator < 0))
#endif
	{
		outputStream << '-';
	}

	if(value.denominator == 1)
	{
		outputStream << abs(value.numerator);
	}
	else
	{
		outputStream << abs(value.numerator) << '/' << abs(value.denominator);
	}

	return outputStream;
}

/*
int CalcObj::getCSVFormatIndex()
{
	static int CSVFormatIndex = ios_base::xalloc();
	return CSVFormatIndex;
}

int CalcObj::getDecimalFractionFormatIndex()
{
	static int decimalFractionFormatIndex = ios_base::xalloc();
	return decimalFractionFormatIndex;
}

int CalcObj::getFullPrecisionFormatIndex()
{
	static int fullPrecisionFormatIndex = ios_base::xalloc();
	return fullPrecisionFormatIndex;
}

*/

int CalcObj::getStreamSettingsFormatIndex()
{
	static int streamSettingsFormatIndex = ios_base::xalloc();
	return streamSettingsFormatIndex;
}

void CalcObj::streamSettingsDestructorCallback(std::ios_base::event eventInfo, std::ios_base& stream, int settingsIndex)
{
	if (eventInfo == std::ios_base::erase_event)
	{
		StreamSettings*& streamData = reinterpret_cast<StreamSettings*&>(stream.pword(settingsIndex));
		delete streamData;
	}
}

CalcObj::StreamSettings* CalcObj::getStreamSettings(ostream& outputStream, bool createSettings)
{
	StreamSettings*& streamData = reinterpret_cast<StreamSettings*&>(outputStream.pword(getStreamSettingsFormatIndex()));

	if (streamData == nullptr)
	{
		if (createSettings)
		{
			streamData = new StreamSettings();

			outputStream.register_callback(&streamSettingsDestructorCallback, getStreamSettingsFormatIndex());
		}
	}

	return streamData;
}

CalcObj::StreamSettings CalcObj::getStreamSettings(std::ostream & outputStream)
{
	StreamSettings* currentStreamSettings = getStreamSettings(outputStream, false);

	if (currentStreamSettings != nullptr)
	{
		return (*currentStreamSettings);
	}
	else
	{
		return StreamSettings();
	}
}

ostream& CalcObj::CSVFormat(ostream& outputStream)
{
	/*
	outputStream.iword(getCSVFormatIndex()) = 1;
	return outputStream;
	*/

	StreamSettings* outputStreamSettings = getStreamSettings(outputStream, true);
	outputStreamSettings->CSVOutputMode = true;

	return outputStream;
}

ostream& CalcObj::nonCSVFormat(ostream& outputStream)
{
	StreamSettings* outputStreamSettings = getStreamSettings(outputStream, true);
	outputStreamSettings->CSVOutputMode = false;

	return outputStream;
}

ostream& CalcObj::fractionFormat(ostream& outputStream)
{
	StreamSettings* outputStreamSettings = getStreamSettings(outputStream, true);
	outputStreamSettings->decFracMode = FRACTION_VALUE;

	return outputStream;
}

ostream& CalcObj::decimalFormat(ostream& outputStream)
{
	StreamSettings* outputStreamSettings = getStreamSettings(outputStream, true);
	outputStreamSettings->decFracMode = DECIMAL_VALUE;

	return outputStream;
}

std::ostream& CalcObj::setFullPrecision(std::ostream& outputStream)
{
	StreamSettings* outputStreamSettings = getStreamSettings(outputStream, true);
	outputStreamSettings->fullPrecision = true;

	return outputStream;
}

std::ostream & CalcObj::unsetFullPrecision(std::ostream & outputStream)
{
	StreamSettings* outputStreamSettings = getStreamSettings(outputStream, true);
	outputStreamSettings->fullPrecision = false;

	return outputStream;
}

/*
bool CalcObj::getCSVOutputMode(std::ostream& outputStream)
{
	StreamSettings* outputStreamSettings = getStreamSettings(outputStream, false);


}

decimalFractionMode CalcObj::getDecimalFractionOutputMode(std::ostream & outputStream)
{
	decimalFractionMode resultEnum = DECIMAL_VALUE;

	switch (outputStream.iword(getDecimalFractionFormatIndex()))
	{
	case 1:
		resultEnum = FRACTION_VALUE;
		break;
	case 0:
		resultEnum = DECIMAL_VALUE;
		break;
	}

	return resultEnum;
}

bool CalcObj::getFullPrecision(std::ostream & outputStream)
{
	return (outputStream.iword(getFullPrecisionFormatIndex()) == 1);
}
*/

/*
void CalcObj::split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
*/

void CalcObj::resetObjects()
{
	this->floatObj = 0;
	this->listObj.clear();
}

CalcObj::CalcObj()
{
	this->type = TYPE_FLOAT;
	this->floatObj = 0;
}

CalcObj::CalcObj(const string& objString, bool preservePrecision)
{
	if(objString.length() >= 2 && objString[0] == '{' && objString[objString.length() - 1] == '}')
	{
		string listStr = objString.substr(1, objString.length() - 2);
		vector<string> listStrings;

		StringUtils::split(listStr, ",", listStrings);

		this->listObj.resize(listStrings.size());

		for (unsigned i = 0; i < listStrings.size(); i++)
		{
#ifdef MULTIPRECISION
			if (preservePrecision)
			{
				long numPrecision = significantFigures(listStrings[i]) + 5;
				this->listObj[i] = calcFloat(listStrings[i], mpfr::digits2bits(numPrecision));
			}
			else
			{
				this->listObj[i] = calcFloat(listStrings[i]);
			}
#else
			stringToFloat(listStrings[i], this->listObj[i]);
			// this->listObj[i] = calcFloat(listStrings[i]);
#endif
		}

		this->type = TYPE_LIST;
	}
	else
	{
		this->type = TYPE_FLOAT;

#ifdef MULTIPRECISION
		if (preservePrecision)
		{
			long numPrecision = significantFigures(objString) + 5;
			this->floatObj = calcFloat(objString, mpfr::digits2bits(numPrecision));
		}
		else
		{
			this->floatObj = calcFloat(objString);
		}
#else
		stringToFloat(objString, this->floatObj);
		// this->floatObj = calcFloat(objString);
#endif
	}
}

CalcObj::CalcObj(const calcFloat& objFloat)
{
	this->type = TYPE_FLOAT;
	this->floatObj = objFloat;
}

#ifdef MULTIPRECISION
CalcObj::CalcObj(const int& objInt)
{
	this->type = TYPE_FLOAT;
	this->floatObj = objInt;
}
#endif

CalcObj CalcObj::operator+(const CalcObj& addObj)
{
	CalcObj newObj(*this);

	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(addObj.type)
		{
		case TYPE_FLOAT:
			newObj.floatObj += addObj.floatObj;
			break;
		default:
			throw type_error("Addition type error");
		}
		break;
	case TYPE_LIST:
		switch(addObj.type)
		{
		case TYPE_LIST:
			newObj += addObj;
			break;
		default:
			throw type_error("Addition type error");
		}
	default:
		throw type_error("Addition type error");
	}

	return newObj;
}

CalcObj CalcObj::operator-(const CalcObj& subObj)
{
	CalcObj newObj(*this);

	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(subObj.type)
		{
		case TYPE_FLOAT:
			newObj.floatObj -= subObj.floatObj;
			break;
		default:
			throw type_error("Subtraction type error");
		}
		break;
	case TYPE_LIST:
		switch(subObj.type)
		{
		case TYPE_LIST:
			newObj -= subObj;
			break;
		default:
			throw type_error("Subtraction type error");
		}
	default:
		throw type_error("Subtraction type error");
	}

	return newObj;
}

CalcObj CalcObj::operator*(const CalcObj& multObj)
{
	CalcObj newObj(*this);

	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(multObj.type)
		{
		case TYPE_FLOAT:
			newObj.floatObj *= multObj.floatObj;
			break;
		case TYPE_LIST:
			newObj *= multObj;
			break;
		default:
			throw type_error("Multiplication type error");
		}
		break;
	case TYPE_LIST:
		switch(multObj.type)
		{
		case TYPE_FLOAT:
			newObj *= multObj;
			break;
		default:
			throw type_error("Multiplication type error");
		}
		break;
	default:
		throw type_error("Multiplication type error");
	}

	return newObj;
}

CalcObj CalcObj::operator/(const CalcObj& divObj)
{
	CalcObj newObj(*this);

	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(divObj.type)
		{
		case TYPE_FLOAT:
			if(divObj.floatObj != 0.0)
			{
				newObj.floatObj /= divObj.floatObj;
			}
			else
			{
				throw DivisionByZeroError();
			}
			break;
		default:
			throw type_error("Division type error");
		}
		break;
	case TYPE_LIST:
		switch(divObj.type)
		{
		case TYPE_FLOAT:
			newObj /= divObj;
			break;
		default:
			throw type_error("Division type error");
		}
		break;
	default:
		throw type_error("Division type error");
	}

	return newObj;
}

CalcObj CalcObj::operator+=(const CalcObj& addObj)
{
	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(addObj.type)
		{
		case TYPE_FLOAT:
			this->floatObj += addObj.floatObj;
			break;
		default:
			throw type_error("Addition type error");
		}
		break;
	case TYPE_LIST:
		switch(addObj.type)
		{
			case TYPE_LIST:
			{
				if(addObj.listObj.size() > this->listObj.size())
				{
					this->listObj.resize(addObj.listObj.size());
				}

				for(unsigned i = 0; (i < this->listObj.size() && i < addObj.listObj.size()); i++)
				{
					this->listObj[i] += addObj.listObj[i];
				}

				break;
			}
			default:
				throw type_error("Addition type error");
		}
		break;
	default:
		throw type_error("Addition type error");
	}

	return *this;
}

CalcObj CalcObj::operator-=(const CalcObj& subObj)
{
	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(subObj.type)
		{
		case TYPE_FLOAT:
			this->floatObj -= subObj.floatObj;
			break;
		default:
			throw type_error("Subtraction type error");
		}
		break;
	case TYPE_LIST:
		switch(subObj.type)
		{
			case TYPE_LIST:
			{
				const unsigned minIndex = (this->listObj.size() > subObj.listObj.size() ? subObj.listObj.size() : this->listObj.size());

				for(unsigned i = 0; i < minIndex; i++)
				{
					this->listObj[i] -= subObj.listObj[i];
				}

				break;
			}
			default:
				throw type_error("Subtraction type error");
		}
		break;
	default:
		throw type_error("Subtraction type error");
	}

	return *this;
}

CalcObj CalcObj::operator*=(const CalcObj& multObj)
{
	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(multObj.type)
		{
		case TYPE_FLOAT:
			this->floatObj *= multObj.floatObj;
			break;
		case TYPE_LIST:
			this->type = TYPE_LIST;
			this->listObj.resize(multObj.listObj.size());

			for(unsigned i = 0; i < multObj.listObj.size(); i++)
			{
				this->listObj[i] = this->floatObj * multObj.listObj[i];
			}

			this->floatObj = 0;

			break;
		default:
			throw type_error("Multiplication type error");
		}
		break;
	case TYPE_LIST:
		switch(multObj.type)
		{
		case TYPE_FLOAT:
			for(unsigned i = 0; i < this->listObj.size(); i++)
			{
				this->listObj[i] *= multObj.floatObj;
			}
			break;
		default:
			throw type_error("Multiplication type error");
		}
		break;
	default:
		throw type_error("Multiplication type error");
	}

	return *this;
}

CalcObj CalcObj::operator/=(const CalcObj& divObj)
{
	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(divObj.type)
		{
		case TYPE_FLOAT:
			if(divObj.floatObj != 0.0)
			{
				this->floatObj /= divObj.floatObj;
			}
			else
			{
				throw DivisionByZeroError();
			}
			break;
		default:
			throw type_error("Division type error");
		}
		break;
	case TYPE_LIST:
		switch(divObj.type)
		{
		case TYPE_FLOAT:
			if(divObj.floatObj != 0.0)
			{
				for(unsigned i = 0; i < this->listObj.size(); i++)
				{
					this->listObj[i] /= divObj.floatObj;
				}
			}
			else
			{
				throw DivisionByZeroError();
			}
			break;
		default:
			throw type_error("Division type error");
		}
		break;
	default:
		throw type_error("Division type error");
	}

	return *this;
}

CalcObj CalcObj::operator=(const CalcObj& assignObj)
{
	this->type = assignObj.type;
	this->resetObjects();

	switch(assignObj.type)
	{
	case TYPE_FLOAT:
		this->floatObj = assignObj.floatObj;
		break;
	case TYPE_LIST:
		this->listObj = assignObj.listObj;
	}

	return *this;
}

CalcObj CalcObj::operator=(const calcFloat& assignObj)
{
	this->type = TYPE_FLOAT;
	this->resetObjects();

	this->floatObj = assignObj;

	return *this;
}

CalcObj CalcObj::operator-()
{
	CalcObj newObj;
	newObj.type = this->type;

	switch(this->type)
	{
	case TYPE_FLOAT:
		newObj.floatObj = -(this->floatObj);

		break;
	case TYPE_LIST:
		newObj.listObj.resize(this->listObj.size());

		for(unsigned i = 0; i < this->listObj.size(); i++)
		{
			newObj.listObj[i] = -(this->listObj[i]);
		}

		break;
	}

	return newObj;
}

calcFloat CalcObj::getVerifiedFloat(string typeErrorMsg)
{
	if(this->type == TYPE_FLOAT)
	{
		return this->floatObj;
	}
	else
	{
		throw type_error(typeErrorMsg.c_str());
	}
}

void CalcObj::set_list(const std::vector<calcFloat>& assignObj)
{
	this->type = TYPE_LIST;
	this->resetObjects();

	this->listObj = assignObj;
}

bool CalcObj::get_list(std::vector<calcFloat>& buffer)
{
	bool returnVal = true;

	if(this->type == TYPE_LIST)
	{
		buffer = this->listObj;
	}
	else
	{
		returnVal = false;
	}

	return returnVal;
}

calcObjType CalcObj::get_type()
{
	return this->type;
}

calcFloat CalcObj::get_list_index(unsigned index)
{
	if(index < this->listObj.size())
	{
		return this->listObj[index];
	}
	else
	{
		throw exception("Invalid list index");
	}
}

void CalcObj::set_list_index(unsigned index, calcFloat value)
{
	if(index < this->listObj.size())
	{
		this->listObj[index] = value;
	}
	else
	{
		throw exception("Invalid list index");
	}
}

unsigned CalcObj::get_list_length()
{
	if(this->type == TYPE_LIST)
	{
		return this->listObj.size();
	}
	else
	{
		throw type_error("List length type error");
	}
}

void CalcObj::concat_list(const CalcObj& concatObj)
{
	if(this->type == TYPE_FLOAT)
	{
		this->set_list(vector<calcFloat>(1, this->floatObj));
	}

	switch(concatObj.type)
	{
	case TYPE_FLOAT:
		this->listObj.insert(this->listObj.end(), concatObj.floatObj);
		break;
	case TYPE_LIST:
		this->listObj.insert(this->listObj.end(), concatObj.listObj.begin(), concatObj.listObj.end());
		break;
	default:
		throw type_error("Concatenation type error");
	}
}

void CalcObj::sublist(std::vector<calcFloat>& buffer, unsigned startIndex, unsigned endIndex)
{
	if(this->type == TYPE_LIST)
	{
		if(startIndex < this->listObj.size() && endIndex < this->listObj.size() && startIndex <= endIndex)
		{
			buffer.resize(endIndex - startIndex + 1);

			for(unsigned i = startIndex; i <= endIndex; i++)
			{
				buffer[i - startIndex] = this->listObj[i];
			}
		}
		else
		{
			throw exception("Invalid sublist indices");
		}
	}
	else
	{
		throw type_error("Sublist type error");
	}
}

#ifdef MULTIPRECISION
CalcObj& CalcObj::set_precision(long newPrecision)
{
	switch (this->type)
	{
	case TYPE_FLOAT:
		if (this->floatObj.getPrecision() != newPrecision)
		{
			this->floatObj.setPrecision(newPrecision);
		}

		break;
	case TYPE_LIST:
		for (unsigned i = 0; i < this->listObj.size(); i++)
		{
			if (this->listObj[i].getPrecision() != newPrecision)
			{
				this->listObj[i].setPrecision(newPrecision);
			}
		}

		break;
	}

	return (*this);
}
#endif

bool CalcObj::operator==(const CalcObj& compareObj)
{
	bool returnVal = true;

	if(this->type != compareObj.type)
	{
		returnVal = false;
	}
	else
	{
		switch(this->type)
		{
		case TYPE_FLOAT:
			returnVal = (this->floatObj == compareObj.floatObj);
			break;
		case TYPE_LIST:
			if(this->listObj.size() != compareObj.listObj.size())
			{
				returnVal = false;
			}
			else
			{
				for(unsigned i = 0; returnVal && i < this->listObj.size(); i++)
				{
					if(this->listObj[i] != compareObj.listObj[i])
					{
						returnVal = false;
					}
				}
			}
			break;
		default:
			returnVal = false;
		}
	}

	return returnVal;
}

bool CalcObj::operator!=(const CalcObj& compareObj)
{
	return !(this->operator==(compareObj));
}

bool CalcObj::operator<(const CalcObj& compareObj)
{
	bool returnVal = false;

	switch(this->type)
	{
	case TYPE_FLOAT:
		switch(compareObj.type)
		{
		case TYPE_FLOAT:
			returnVal = (this->floatObj < compareObj.floatObj);
			break;
		default:
			throw type_error("Comparison type error");
		}
		break;
	case TYPE_LIST:
		switch(compareObj.type)
		{
		case TYPE_LIST:
			if(this->listObj.size() != compareObj.listObj.size())
			{
				returnVal = (this->listObj.size() < compareObj.listObj.size());
			}
			else
			{
				bool exitComparison = false;

				for(unsigned i = 0; !exitComparison && i < this->listObj.size(); i++)
				{
					if(this->listObj[i] != compareObj.listObj[i])
					{
						if(this->listObj[i] < compareObj.listObj[i])
						{
							returnVal = true;
						}

						exitComparison = true;
					}
				}
			}
			break;
		default:
			throw type_error("Comparison type error");
		}
		break;
	default:
		throw type_error("Comparison type error");
	}

	return returnVal;
}

bool CalcObj::operator>(const CalcObj& compareObj)
{
	return !((*this < compareObj) || (*this == compareObj));
}

bool CalcObj::operator<=(const CalcObj& compareObj)
{
	return !(*this > compareObj);
}

bool CalcObj::operator>=(const CalcObj& compareObj)
{
	return !(*this < compareObj);
}

/*
void CalcObj::getString(string& destination, decimalFractionMode decimalFractionFormat, bool csvMode)
{
	switch (this->type)
	{
	case TYPE_FLOAT:
		if (decimalFractionFormat == decimalFractionMode::DECIMAL_VALUE)
		{
#ifdef MULTIPRECISION
			destination = this->floatObj.toString();
#endif
			out << outObj.floatObj;
		}
		else if (CalcObj::getDecimalFractionOutputMode(out) == decimalFractionMode::FRACTION_VALUE)
		{
			Fraction conversionResult;

			if (floatToFraction(&conversionResult, outObj.floatObj, calcSignedInt(10000), calcFloat("1E-45")))
			{
				out << conversionResult;
			}
			else
			{
#ifdef MULTIPRECISION
				out << setprecision(mpfr::bits2digits(outObj.floatObj.getPrecision()));
#endif
				out << outObj.floatObj;
			}
		}
		break;
	case TYPE_LIST:
		if (!csvMode)
		{
			out << '{';
		}
		else
		{
			out << "BEGIN_LIST,";
		}

		for (unsigned i = 0; i < outObj.listObj.size(); i++)
		{
			if (CalcObj::getDecimalFractionOutputMode(out) == decimalFractionMode::DECIMAL_VALUE)
			{
#ifdef MULTIPRECISION
				out << setprecision(mpfr::bits2digits(outObj.listObj[i].getPrecision()));
#endif
				out << outObj.listObj[i];
			}
			else if (CalcObj::getDecimalFractionOutputMode(out) == decimalFractionMode::FRACTION_VALUE)
			{
				Fraction conversionResult;

				if (floatToFraction(&conversionResult, outObj.listObj[i], calcSignedInt(10000), calcFloat("1E-45")))
				{
					out << conversionResult;
				}
				else
				{
#ifdef MULTIPRECISION
					out << setprecision(mpfr::bits2digits(outObj.listObj[i].getPrecision()));
#endif
					out << outObj.listObj[i];
				}
			}

			if (CalcObj::getCSVOutputMode(out) || i < (outObj.listObj.size() - 1))
			{
				out << ',';
			}
		}

		if (!CalcObj::getCSVOutputMode(out))
		{
			out << '}';
		}
		else
		{
			out << "END_LIST";
		}
	}

	return out;
}
*/

std::ostream& operator<<(std::ostream& out, const CalcObj& outObj)
{
	const CalcObj::StreamSettings* outputSettings = CalcObj::getStreamSettings(out, false);

	streamsize origPrecision = out.precision();
#ifdef MULTIPRECISION
	// calcFloat("1E-45")
	calcFloat fractionTolerance = 8.0;
#else
	if (outputSettings != nullptr && outputSettings->fullPrecision)
	{
		out.precision(numeric_limits<calcFloat>::digits10);
	}
	
	calcFloat fractionTolerance = 8.0;
#endif
		switch(outObj.type)
		{
		case TYPE_FLOAT:
			if (outputSettings == nullptr || outputSettings->decFracMode == decimalFractionMode::DECIMAL_VALUE)
			{
#ifdef MULTIPRECISION
				if (outputSettings != nullptr && outputSettings->fullPrecision)
				{
					out << setprecision(mpfr::bits2digits(outObj.floatObj.getPrecision()));
				}
#endif
				out << outObj.floatObj;
			}
			else if(outputSettings != nullptr && outputSettings->decFracMode == decimalFractionMode::FRACTION_VALUE)
			{
				Fraction conversionResult;

				if(floatToFraction(&conversionResult, outObj.floatObj, calcSignedInt(10000), fractionTolerance))
				{
					out << conversionResult;
				}
				else
				{
#ifdef MULTIPRECISION
					if (outputSettings != nullptr && outputSettings->fullPrecision)
					{
						out << setprecision(mpfr::bits2digits(outObj.floatObj.getPrecision()));
					}
#endif
					out << outObj.floatObj;
				}
			}
			break;
		case TYPE_LIST:
			if(outputSettings != nullptr && outputSettings->CSVOutputMode)
			{
				out << "BEGIN_LIST,";
			}
			else
			{
				out << '{';
			}

			for(unsigned i = 0; i < outObj.listObj.size(); i++)
			{
				if (outputSettings == nullptr || outputSettings->decFracMode == decimalFractionMode::DECIMAL_VALUE)
				{
#ifdef MULTIPRECISION
					if (outputSettings != nullptr && outputSettings->fullPrecision)
					{
						out << setprecision(mpfr::bits2digits(outObj.listObj[i].getPrecision()));
					}
#endif
					out << outObj.listObj[i];
				}
				else if (outputSettings != nullptr && outputSettings->decFracMode == decimalFractionMode::FRACTION_VALUE)
				{
					Fraction conversionResult;

					if (floatToFraction(&conversionResult, outObj.listObj[i], calcSignedInt(10000), fractionTolerance))
					{
						out << conversionResult;
					}
					else
					{
#ifdef MULTIPRECISION
						if (outputSettings != nullptr && outputSettings->fullPrecision)
						{
							out << setprecision(mpfr::bits2digits(outObj.listObj[i].getPrecision()));
						}
#endif
						out << outObj.listObj[i];
					}
				}

				if((outputSettings != nullptr && outputSettings->CSVOutputMode) || i < (outObj.listObj.size() - 1))
				{
					out << ',';
				}
			}

			if(outputSettings != nullptr && outputSettings->CSVOutputMode)
			{
				out << "END_LIST";
			}
			else
			{
				out << '}';
			}
		}

		out.precision(origPrecision);

		return out;
}

string CalcObj::getString(StreamSettings settings, unsigned precision) const
{
	stringstream conversionStream;
	conversionStream.pword(getStreamSettingsFormatIndex()) = reinterpret_cast<void*>(&settings);

	if (!settings.fullPrecision)
	{
		conversionStream.precision(precision);
	}

	conversionStream << (*this);
	return (conversionStream.str());
}

CalcObj pow(const CalcObj& base, const CalcObj& power)
{
	CalcObj newObj(base);

	switch(base.type)
	{
	case TYPE_FLOAT:
		switch(power.type)
		{
		case TYPE_FLOAT:
			newObj.floatObj = pow(base.floatObj, power.floatObj);
			break;
		default:
			throw type_error("Power type error");
		}
		break;
	default:
		throw type_error("Power type error");
	}

	return newObj;
}

CalcObj fmod(const CalcObj& num, const CalcObj& denom)
{
	CalcObj newObj(num);

	switch(num.type)
	{
	case TYPE_FLOAT:
		switch(denom.type)
		{
		case TYPE_FLOAT:
			newObj.floatObj = fmod(num.floatObj, denom.floatObj);
			break;
		default:
			throw type_error("Modulo type error");
		}
		break;
	default:
		throw type_error("Modulo type error");
	}

	return newObj;
}