#pragma once
#include <string>
#include <vector>

namespace Crimson
{
	std::wstring ToWString(const std::string& aString);
	std::string ToString(const std::wstring& aString);

	std::string ToLower(const std::string& aString);
	std::string ToUpper(const std::string& aString);

	std::string RemoveStringPart(const std::string& aString, const std::string& aPartToRemove);
	std::vector<std::string> SplitString(const std::string& anInput, const char aSplitter);
}