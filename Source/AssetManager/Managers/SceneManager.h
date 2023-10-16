#pragma once
#include "../Assets/GameObject.h"
#include <unordered_set>

struct Scene
{
	unsigned GameObjectIDCount = 0;
	std::string Name{};
	std::unordered_map<unsigned, GameObject> GameObjects;

	Scene() = default;
	Scene(const Json::Value& aJson);
	Scene(const Scene&) = default;
	Scene(Scene&&) = default;
	~Scene() = default;
	operator Json::Value() const;
	Scene& operator=(const Scene&) = default;
	Scene& operator=(Scene&&) = default;
};
std::istream& operator>>(std::istream& aStream, Scene& aScene);
std::ostream& operator<<(std::ostream& aStream, const Scene& aScene);

struct EditorScene
{
	unsigned GameObjectIDCount = 0;
	std::string Name{};
	std::string Path{};
	std::unordered_map<unsigned, std::shared_ptr<GameObject>> GameObjects;

	EditorScene() = default;
	EditorScene(const Json::Value& aJson);
	EditorScene(const EditorScene&) = default;
	EditorScene(EditorScene&&) = default;
	~EditorScene() = default;
	operator Json::Value() const;
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

	const std::unordered_set<std::string>& GetScenelist();

	Scene GetScene(const std::string& aPath, bool aShouldLogErrors);
	EditorScene GetEditorScene(const std::string& aPath, bool aShouldLogErrors);

	void SaveScene(const std::string& aPath, const Scene& aScene, bool anAsBinary);
	void SaveScene(const std::string& aPath, const EditorScene& aScene, bool anAsBinary);

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

	Scene LoadScene(const std::string& aPath, bool aShouldLogErrors);
	Scene LoadBinaryScene(const std::string& aPath, bool aShouldLogErrors);

	EditorScene LoadEditorScene(const std::string& aPath, bool aShouldLogErrors);
	EditorScene LoadBinaryEditorScene(const std::string& aPath, bool aShouldLogErrors);

	void SaveSceneToFile(const std::string& aPath, const Scene& aScene);
	void SaveSceneToBinary(const std::string& aPath, const Scene& aScene);

	void SaveSceneToFile(const std::string& aPath, const EditorScene& aScene);
	void SaveSceneToBinary(const std::string& aPath, const EditorScene& aScene);
};