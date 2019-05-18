#pragma once

#include <string>
#include <vector>
#include <codecvt>

namespace StringUtils
{
	void split(const std::string& sourceString, const std::string& delimString, std::vector<std::string>& elemsDestination, unsigned maxElements = 0);
	std::wstring UTF8StringToWString(const std::string& inputString);
}