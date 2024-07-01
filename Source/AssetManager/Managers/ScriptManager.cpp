#include "AssetManager.pch.h"
#include "ScriptManager.h"

Blueprint* ScriptManager::LoadScript(const std::string& aPath, bool aShouldLogErrors)
{
	std::ifstream file(aPath);
	const std::string inGraph = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	Blueprint* graph = nullptr;
	if (!inGraph.empty())
	{
		auto iter = myScripts.emplace(aPath, Blueprint(inGraph)).first;
		graph = &iter->second;
		graph->myPath = &iter->first;
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("MaterialManager: Could not open file at: " + aPath);
		}
		return nullptr;
	}

	return graph;
}
