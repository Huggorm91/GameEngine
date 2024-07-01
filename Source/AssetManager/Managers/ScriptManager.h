#pragma once
#include "ThirdParty/MuninGraph/ScriptGraph/Blueprint.h"
#include <unordered_map>
#include <unordered_set>

class ScriptManager
{
public:
	ScriptManager() = default;
	~ScriptManager() = default;

	void Init();

	Blueprint* GetScript(const std::string& aPath, bool aShouldLogErrors);

	static inline const char* GetExtension(){ return ".blp"; }
	static inline const char* GetPath(){ return "..\\Content\\Blueprints\\"; }
	static inline const wchar_t* GetExtensionW(){ return L".blp"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Blueprints\\"; }

private:
	std::unordered_map<std::string, Blueprint> myScripts;
	std::unordered_set<std::string> myFilePaths;

	Blueprint* LoadScript(const std::string& aPath, bool aShouldLogErrors);
};