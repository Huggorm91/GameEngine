#pragma once
#include "../Assets/Prefab.h"
#include <unordered_set>

class PrefabManager
{
public:
	PrefabManager() = default;
	~PrefabManager() = default;

	void Init();

	Prefab GetPrefab(const std::string& aPath, bool aShouldLogErrors);
	GameObject* GetTemplate(const std::string& aPath, bool aShouldLogErrors);
	const std::unordered_set<std::string>& GetPrefablist() const;

	void CreatePrefab(const std::string& aPath, const GameObject& aPrefab);

	void SavePrefab(const std::string& aPath) const;
	void SavePrefab(const std::string& aPath, const GameObject& aPrefab);
	void SaveAllPrefabs() const;

	static inline const char* GetExtension(){ return ".prfb"; }
	static inline const char* GetPath(){ return "..\\Content\\Prefabs\\"; }
	static inline const wchar_t* GetExtensionW(){ return L".prfb"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Prefabs\\"; }

private:
	std::unordered_map<std::string, GameObject> myPrefabs;
	std::unordered_set<std::string> myValidPaths;
	std::unordered_set<std::string> myUnloadedFilePaths;

	GameObject* LoadPrefab(const std::string& aPath, bool aShouldLogErrors);
	void SavePrefabToFile(const std::string& aPath, const GameObject& aPrefab) const;
	std::string ValidatePath(const std::string& aPath) const;
};