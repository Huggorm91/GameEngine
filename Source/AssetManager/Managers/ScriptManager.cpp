#include "AssetManager.pch.h"
#include "ScriptManager.h"

void ScriptManager::Init()
{
	UpdateFilePaths();
}

void ScriptManager::UpdateFilePaths()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension(), true);
}

const std::unordered_set<std::string>& ScriptManager::GetScriptlist()
{
	return myFilePaths;
}

Script::ScriptData* ScriptManager::GetScript(const std::string& aPath, bool aShouldLogErrors)
{
	if (auto iter = myScripts.find(aPath); iter != myScripts.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadScript(aPath, aShouldLogErrors);
	}
}

Script::ScriptData* ScriptManager::ForceLoadScript(const std::string& aPath, bool aShouldLogErrors)
{
	return LoadScript(aPath, aShouldLogErrors);
}

void ScriptManager::SaveScript(const Script::ScriptData& aScript, const std::string& aPath)
{
	std::string path = aPath;
	if (!Crimson::HasValidExtension(aPath, GetExtension()))
	{
		path += GetExtension();
	}
	path = Crimson::CreateValidPath(path, GetPath());

	if (path.empty())
	{
		AMLogger.Warn("ScriptManager: Could not save script to path: " + aPath);
		return;
	}

	std::ofstream file(path, std::ios::binary | std::ios::trunc);
	if (file)
	{
		size_t size = aScript.size();
		file.write(reinterpret_cast<const char*>(&size), sizeof(size));
		file.write(reinterpret_cast<const char*>(aScript.data()), size);
		file.flush();
	}
	else
	{
		AMLogger.Err("ScriptManager: Could not open file at: " + aPath);
	}

	file.close();
}

Script::ScriptData* ScriptManager::LoadScript(const std::string& aPath, bool aShouldLogErrors)
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = Crimson::GetValidPath(path, GetPath());
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("MaterialManager: Could not load material from path: " + aPath);
		}
		return nullptr;
	}

	Script::ScriptData data;
	std::ifstream file(path, std::ios::binary);
	if (file)
	{
		size_t size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));
		data.resize(size);
		file.read(reinterpret_cast<char*>(data.data()), size);
		file.close();
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("ScriptManager: Could not open file at: " + aPath);
		}	
		file.close();
		return nullptr;
	}	

	auto iter = myScripts.emplace(aPath, data);

	return &iter.first->second;
}
