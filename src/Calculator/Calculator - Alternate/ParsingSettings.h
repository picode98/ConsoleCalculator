#pragma once
#include <set>
#include <string>

struct ParsingSettings
{
	enum angleMode { degrees, radians, gradians };

	angleMode parseAngleMode = angleMode::radians;
	std::set<std::string> functionBlacklist;
};