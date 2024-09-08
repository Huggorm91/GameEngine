#pragma once
#include "GameplayEngine/Scene/SceneLoader.h"
#include <memory>
#include <future>

class ThreadPool;
class ObjectManager;

class SceneManager
{
public:
	SceneManager(ObjectManager* anObjectManager, ThreadPool* aThreadPool);
	~SceneManager() = default;

	bool IsLoadingScene() const;
	bool HasFinnishedLoading() const;

	void ActivateLoadedScene();
	void ClearLoadedScene();

	bool HasActiveScene() const;
	Scene& GetActiveScene();
	void UnloadActiveScene();

	void LoadScene(const std::string& aPath);
	void LoadSceneAsync(const std::string& aPath);
	void SaveScene(const std::string& aPath, const Scene& aScene, bool anAsBinary);

private:
	bool myIsLoadingScene;
	ThreadPool* myThreadPool;
	ObjectManager* myObjectManager;
	std::unique_ptr<Scene> myActiveScene;
	std::future<Scene> myFuture;
	SceneLoader mySceneLoader;
};