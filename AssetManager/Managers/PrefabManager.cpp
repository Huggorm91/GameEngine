#include "AssetManager.pch.h"
#include "PrefabManager.h"
#include <External/jsonCpp/json.h>
#include "../DirectoryFunctions.h"

PrefabManager::PrefabManager(const std::string& aPath): myUnloadedFilePaths(), myPath(aPath), myPrefabs()
{
}

void PrefabManager::Init()
{
	myUnloadedFilePaths = GetAllFilepathsInDirectory(myPath, GetExtension());
	
	for (auto& path : myUnloadedFilePaths)
	{
		myValidPaths.emplace(RemoveStringPart(path, myPath));
	}

	myPrefabs.emplace("Empty", GameObject(0));
	myValidPaths.emplace("Empty");
}

Prefab PrefabManager::GetPrefab(const std::string& aPath)
{
	if (auto iter = myPrefabs.find(aPath); iter != myPrefabs.end())
	{
		return { &iter->second, aPath };
	}
	else
	{
		return { LoadPrefab(aPath), aPath };
	}
}

GameObject* PrefabManager::GetTemplate(const std::string& aPath)
{
	if (auto iter = myPrefabs.find(aPath); iter != myPrefabs.end())
	{
		return &iter->second;
	}
	else
	{
		std::string path = ValidatePath(aPath);
		if (auto iter = myUnloadedFilePaths.find(path); iter != myUnloadedFilePaths.end())
		{
			return LoadPrefab(aPath);
		}
		else
		{
			myValidPaths.emplace(aPath);
			myPrefabs.emplace(aPath, GameObject(0));
			return &myPrefabs.at(aPath);
		}		
	}
}

const std::unordered_set<std::string>& PrefabManager::GetPrefablist() const
{
	return myValidPaths;
}

void PrefabManager::CreatePrefab(const std::string& aPath, const GameObject& aPrefab)
{
	if (auto iter = myPrefabs.find(aPath); iter != myPrefabs.end())
	{
		iter->second = aPrefab;
	}
	else
	{
		std::string path = ValidatePath(aPath);
		if (auto iter = myUnloadedFilePaths.find(path); iter != myUnloadedFilePaths.end())
		{
			myUnloadedFilePaths.erase(iter);
		}
		myValidPaths.emplace(aPath);
		myPrefabs.emplace(aPath, GameObject(0));
		myPrefabs.at(aPath) = aPrefab;
	}
}

void PrefabManager::SavePrefab(const std::string& aPath) const
{
	if (auto iter = myPrefabs.find(aPath); iter != myPrefabs.end())
	{
		SavePrefabToFile(iter->first, iter->second);
	}
}

void PrefabManager::SavePrefab(const std::string& aPath, const GameObject& aPrefab)
{
	GameObject* prefabPtr = nullptr;
	if (auto iter = myPrefabs.find(aPath); iter != myPrefabs.end())
	{
		prefabPtr = &iter->second;
	}
	else
	{
		myValidPaths.emplace(aPath);
		myPrefabs.emplace(aPath, GameObject(0));
		prefabPtr = &myPrefabs.at(aPath);
		*prefabPtr = aPrefab;
	}
	SavePrefabToFile(aPath, *prefabPtr);
}

void PrefabManager::SaveAllPrefabs() const
{
	for (auto& [path, prefab] : myPrefabs)
	{
		SavePrefabToFile(path, prefab);
	}
}

GameObject* PrefabManager::LoadPrefab(const std::string& aPath)
{

	std::string path = ValidatePath(aPath);
	if (path.empty())
	{
		AMLogger.Err("PrefabManager: Could not load prefab from path: " + aPath);
		return nullptr;
	}

	Json::Value json;
	std::fstream fileStream(path, std::ios::in);
	if (fileStream)
	{
		fileStream >> json;
	}
	else
	{
		AMLogger.Err("PrefabManager: Could not open file at: " + aPath);
		fileStream.close();
		return nullptr;
	}
	fileStream.close();

	myUnloadedFilePaths.erase(path);
	auto iter = myPrefabs.emplace(aPath, json);

	return &iter.first->second;
}

void PrefabManager::SavePrefabToFile(const std::string& aPath, const GameObject& aPrefab) const
{
	std::string path = aPath;
	if (!HasValidExtension(aPath, GetExtension()))
	{
		path += GetExtension();
	}
	path = CreateValidPath(path, myPath, &AMLogger);

	if (path.empty())
	{
		AMLogger.Err("PrefabManager: Could not save prefab to path: " + aPath);
		return;
	}

	std::fstream fileStream(path, std::ios::out);
	if (fileStream)
	{
		Json::Value prefab = aPrefab.ToJson();
		prefab["ID"] = 0;
		Json::StreamWriterBuilder builder;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(prefab, &fileStream);
		fileStream.flush();
	}
	else
	{
		AMLogger.Err("PrefabManager: Could not open file at: " + aPath);
	}
	fileStream.close();
}

std::string PrefabManager::ValidatePath(const std::string& aPath) const
{
	std::string path = AddExtensionIfMissing(aPath, GetExtension());
	return GetValidPath(path, myPath, &AMLogger);
}
