#pragma once
#include "Scene.h"
#include <unordered_set>

class SceneLoader
{
public:
	SceneLoader() = default;
	~SceneLoader() = default;

	void Init();

	void UpdateFilePaths();
	const std::unordered_set<std::string>& GetScenelist();

	Scene GetScene(const std::string& aPath) const;

	void SaveScene(const std::string& aPath, const Scene& aScene, bool anAsBinary);

	static inline const char* GetExtension(){ return ".scene"; }
	static inline const char* GetBinaryExtension(){ return ".lvl"; }
	static inline const char* GetPath(){ return "..\\Content\\Scenes\\"; }

	static inline const wchar_t* GetExtensionW(){ return L".scene"; }
	static inline const wchar_t* GetBinaryExtensionW(){ return L".lvl"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Scenes\\"; }

private:
	std::unordered_set<std::string> myFilePaths;

	std::string ValidatePath(const std::string& aPath) const;
	std::string ValidateSavePath(const std::string& aPath, const std::string& anExtension) const;

	Scene LoadScene(const std::string& aPath) const;
	Scene LoadBinaryScene(const std::string& aPath) const;

	void SaveSceneToFile(const std::string& aPath, const Scene& aScene);
	void SaveSceneToBinary(const std::string& aPath, const Scene& aScene);
};