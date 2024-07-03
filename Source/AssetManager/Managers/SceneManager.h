#pragma once
#include "../Assets/GameObject.h"
#include <unordered_set>

class ScriptGraph;

struct Scene
{
	unsigned gameObjectIDCount;
	std::shared_ptr<ScriptGraph> scriptGraph;
	std::string name;
	std::unordered_map<unsigned, GameObject> gameObjects;

	Scene();
	Scene(const Json::Value& aJson);
	Scene(const Scene&) = default;
	Scene(Scene&&) = default;
	~Scene() = default;
	Scene& operator=(const Scene&) = default;
	Scene& operator=(Scene&&) = default;
};
std::istream& operator>>(std::istream& aStream, Scene& aScene);
std::ostream& operator<<(std::ostream& aStream, const Scene& aScene);

struct EditorScene
{
	unsigned gameObjectIDCount;
	std::shared_ptr<ScriptGraph> scriptGraph;
	std::string name;
	std::string path;
	std::unordered_map<unsigned, std::shared_ptr<GameObject>> gameObjects;

	EditorScene();
	EditorScene(const Json::Value& aJson);
	EditorScene(const EditorScene&) = default;
	EditorScene(EditorScene&&) = default;
	~EditorScene() = default;
	EditorScene& operator=(const EditorScene&) = default;
	EditorScene& operator=(EditorScene&&) = default;
};
std::istream& operator>>(std::istream& aStream, EditorScene& aScene);
std::ostream& operator<<(std::ostream& aStream, const EditorScene& aScene);

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

	void Init();

	void UpdateFilePaths();
	const std::unordered_set<std::string>& GetScenelist();

	Scene GetScene(const std::string& aPath, bool aShouldLogErrors);
	EditorScene GetEditorScene(const std::string& aPath, bool aShouldLogErrors);

	void SaveScene(const std::string& aPath, const Scene& aScene, bool anAsBinary);
	void SaveScene(const std::string& aPath, const EditorScene& aScene, bool anAsBinary);

	consteval static inline const char* GetExtension(){ return ".scene"; }
	consteval static inline const char* GetBinaryExtension(){ return ".lvl"; }
	consteval static inline const char* GetPath(){ return "..\\Content\\Scenes\\"; }

	consteval static inline const wchar_t* GetExtensionW(){ return L".scene"; }
	consteval static inline const wchar_t* GetBinaryExtensionW(){ return L".lvl"; }
	consteval static inline const wchar_t* GetPathW(){ return L"..\\Content\\Scenes\\"; }

private:
	std::unordered_set<std::string> myFilePaths;

	std::string ValidatePath(const std::string& aPath) const;
	std::string ValidateSavePath(const std::string& aPath, const std::string& anExtension) const;

	Scene LoadScene(const std::string& aPath, bool aShouldLogErrors);
	Scene LoadBinaryScene(const std::string& aPath, bool aShouldLogErrors);

	EditorScene LoadEditorScene(const std::string& aPath, bool aShouldLogErrors);
	EditorScene LoadBinaryEditorScene(const std::string& aPath, bool aShouldLogErrors);

	void SaveSceneToFile(const std::string& aPath, const Scene& aScene);
	void SaveSceneToBinary(const std::string& aPath, const Scene& aScene);

	void SaveSceneToFile(const std::string& aPath, const EditorScene& aScene);
	void SaveSceneToBinary(const std::string& aPath, const EditorScene& aScene);
};