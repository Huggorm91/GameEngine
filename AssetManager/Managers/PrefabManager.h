#pragma once
#include "../Assets/Prefab.h"
#include <unordered_set>

class PrefabManager
{
public:
	PrefabManager() = default;
	PrefabManager(const std::string& aPath);
	~PrefabManager() = default;

	void Init();

	Prefab GetPrefab(const std::string& aPath);
	GameObject* GetTemplate(const std::string& aPath);
	const std::unordered_set<std::string>& GetPrefablist() const;

	void CreatePrefab(const std::string& aPath, const GameObject& aPrefab);

	void SavePrefab(const std::string& aPath) const;
	void SavePrefab(const std::string& aPath, const GameObject& aPrefab);
	void SaveAllPrefabs() const;

	inline const std::string& GetPath(){ return myPath; }
	static inline std::string GetExtension(){ return ".prfb"; }

private:
	std::unordered_map<std::string, GameObject> myPrefabs;
	std::unordered_set<std::string> myValidPaths;
	std::unordered_set<std::string> myUnloadedFilePaths;
	const std::string myPath = "Content\\Prefabs\\";

	GameObject* LoadPrefab(const std::string& aPath);
	void SavePrefabToFile(const std::string& aPath, const GameObject& aPrefab) const;
	std::string ValidatePath(const std::string& aPath) const;
};