#include "StringUtils.h"

using std::string;
using std::wstring;
using std::vector;

wstring StringUtils::UTF8StringToWString(const string& inputString)
{
	using std::wstring_convert;
	using std::codecvt_utf8;

	wstring_convert<codecvt_utf8<wchar_t>> stringConverter;
	return stringConverter.from_bytes(inputString);
}

void StringUtils::split(const string& sourceString, const string& delimString, vector<string>& elemsDestination, unsigned maxElements)
{
	unsigned searchStartIndex = 0,
		numElements = 0;

	size_t resultStartIndex = 0;

	while (searchStartIndex < sourceString.size()
		&& (maxElements == 0 || numElements < (maxElements - 1))
		&& (resultStartIndex != string::npos))
	{
		resultStartIndex = sourceString.find(delimString, searchStartIndex);

		if (resultStartIndex != string::npos)
		{
			elemsDestination.push_back(sourceString.substr(searchStartIndex, resultStartIndex - searchStartIndex));
			searchStartIndex = resultStartIndex + delimString.size();
			numElements++;
		}
	}

	if (searchStartIndex < sourceString.size())
	{
		elemsDestination.push_back(sourceString.substr(searchStartIndex));
	}
}