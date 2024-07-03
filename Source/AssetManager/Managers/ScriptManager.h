#pragma once
#pragma warning(push, 1)
#pragma warning(disable:4244)
#include "ThirdParty/MuninGraph/MuninGraph.h"
#pragma warning(pop)

namespace Script
{
	typedef std::vector<uint8_t> ScriptData;
}

class ScriptManager
{
public:
	ScriptManager() = default;
	~ScriptManager() = default;

	void Init();

	void UpdateFilePaths();
	const std::unordered_set<std::string>& GetScriptlist();

	Script::ScriptData* GetScript(const std::string& aPath, bool aShouldLogErrors);
	Script::ScriptData* ForceLoadScript(const std::string& aPath, bool aShouldLogErrors);

	void SaveScript(const Script::ScriptData& aScript, const std::string& aPath);

	consteval static inline const char* GetExtension() { return ".scrp"; }
	consteval static inline const char* GetPath() { return "..\\Content\\Scripts\\"; }
	consteval static inline const wchar_t* GetExtensionW() { return L".scrp"; }
	consteval static inline const wchar_t* GetPathW() { return L"..\\Content\\Scripts\\"; }

private:
	std::unordered_map<std::string, Script::ScriptData> myScripts;
	std::unordered_set<std::string> myFilePaths;

	Script::ScriptData* LoadScript(const std::string& aPath, bool aShouldLogErrors);
};