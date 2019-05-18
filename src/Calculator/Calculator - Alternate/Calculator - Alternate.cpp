#include <iostream>
#include <string>

#define NOMINMAX

#include <Windows.h>

#include "GlobalBuildVariables.h"
#include "ConsoleCalculator.h"
#include "resource.h"

using namespace std;

wstring getApplicationDirectory();
bool checkFlag(int argc, const char* argv[], const char* flagStr);

#ifdef MULTIPRECISION
const wstring CONFIG_FILE_NAME = L"arbitrary_precision_config.xml";
#else
const wstring CONFIG_FILE_NAME = L"double_precision_config.xml";
#endif

const wstring HELP_FILE_NAME = L"calculator_help.txt";

int main(int argc, char* argv[])
{
	wstring appDir = getApplicationDirectory(),
			configPath = appDir + L"\\" + CONFIG_FILE_NAME,
			helpFilePath = appDir + L"\\" + HELP_FILE_NAME;

	ConsoleCalculator calcApplication(helpFilePath);

	if(argc <= 1)
	{
		cout << VER_PRODUCT_NAME_STR << " version " << VER_PRODUCT_VERSION_STR << " by " << VER_COMPANY_NAME_STR << " (" << VER_PRECISION_DESCRIPTION_STR << " build)" << endl
			 << "This program was released under the " << VER_LICENSE_STR << '.' << endl
			 << "Enter 'exit' to quit, 'h' for complete documentation, or any mathematical expression to evaluate it." << endl;

		calcApplication.setConfiguration(configPath);

		// While the user does not type 'q' (quit shortcut), parse strings the user enters
		calcApplication.runPromptLoop();
	}
	else
	{
		calcApplication.setConfiguration(configPath);

		if(argc <= 2)
		{
			calcApplication.parseUserString(string(argv[1]));
		}
		else if(checkFlag(argc, (const char**)argv, "-csv"))
		{
			cout << calcObj::CSVFormat;

			calcApplication.parseUserString(string(argv[1]), true);
		}
	}
}

wstring getApplicationDirectory()
{
	// Allocate buffer (see http://stackoverflow.com/a/7048683/3348992)
	wchar_t appPathBuf[FILENAME_MAX + 1];
	// LPWSTR appPathBuf = new TCHAR[FILENAME_MAX + 1];

	// Get application path (see http://stackoverflow.com/a/198099/3348992)
	GetModuleFileNameW(nullptr, appPathBuf, FILENAME_MAX + 1);

	// Convert application path to wstring
	wstring fileName(appPathBuf);

	// Strip application folder from application path (see http://stackoverflow.com/a/10364927/3348992)
	fileName = fileName.substr(0, fileName.find_last_of(L"\\/"));

	return fileName;
}

bool checkFlag(int argc, const char* argv[], const char* flagStr)
{
	bool returnVal = false;

	for(int i = 0; !returnVal && i < argc; i++)
	{
		if(strcmp(argv[i], flagStr) == 0)
		{
			returnVal = true;
		}
	}

	return returnVal;
}