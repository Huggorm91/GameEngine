#include "StringFunctions.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <algorithm>
#include <assert.h>
#include <sstream>

namespace Crimson
{
	std::wstring ToWString(const std::string& aString)
	{
		const int sLength = static_cast<int>(aString.length());
		const int len = MultiByteToWideChar(CP_ACP, 0, aString.c_str(), sLength, 0, 0);
		std::wstring result(len, L'\0');
		MultiByteToWideChar(CP_ACP, 0, aString.c_str(), sLength, &result[0], len);
		return result;
	}

	std::string ToString(const std::wstring& aString)
	{
		const int sLength = static_cast<int>(aString.length());
		const int len = WideCharToMultiByte(CP_ACP, 0, aString.c_str(), sLength, 0, 0, 0, 0);
		std::string result(len, L'\0');
		WideCharToMultiByte(CP_ACP, 0, aString.c_str(), sLength, &result[0], len, 0, 0);
		return result;
	}

	std::string ToLower(const std::string& aString)
	{
		std::string lowerCase = aString;
		std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return lowerCase;
	}

	std::string ToUpper(const std::string& aString)
	{
		std::string upperCase = aString;
		std::transform(upperCase.begin(), upperCase.end(), upperCase.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
		return upperCase;
	}

	std::string RemoveStringPart(const std::string& aString, const std::string& aPartToRemove)
	{
		assert(aString.size() >= aPartToRemove.size() && "Trying to remove too much!");
		auto index = aString.find(aPartToRemove);
		if (index == std::string::npos)
		{
			return aString;
		}

		std::string result(aString);
		result.erase(index, aPartToRemove.size());
		return result;
	}

	std::vector<std::string> SplitString(const std::string& anInput, const char aSplitter)
	{
		std::istringstream iss(anInput);
		std::string item;

		std::vector<std::string> output;

		while (std::getline(iss, item, aSplitter))
		{
			output.push_back(item);
		}
		return output;
	}
}