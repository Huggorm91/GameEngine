#include "GameplayEngine.pch.h"
#include "SceneManager.h"
#include "Engine.h"
#include "ObjectManager.h"
#include "../Threadpool/ThreadPool.h"
#include <chrono>

SceneManager::SceneManager(): myIsLoadingScene(false)
{
	mySceneLoader.Init();
}

bool SceneManager::IsLoadingScene() const
{
	return myIsLoadingScene;
}

bool SceneManager::HasFinnishedLoading() const
{
	if (myIsLoadingScene && myFuture.valid())
	{
		return myFuture.wait_until(std::chrono::system_clock::time_point::min()) == std::future_status::ready;
	}
	return false;
}

void SceneManager::ActivateLoadedScene()
{
	assert(HasFinnishedLoading() && "Scene was not loaded!");
	try
	{
		myActiveScene = std::make_unique<Scene>(myFuture.get());
		Engine::GetObjectManager().SetSceneObjects(&myActiveScene->gameObjects);
	}
	catch (const std::exception&)
	{
		std::throw_with_nested(std::runtime_error("Failed to activate loaded scene!"));
	}	
}

void SceneManager::ClearLoadedScene()
{
	myIsLoadingScene = false;
}

bool SceneManager::HasActiveScene() const
{
	return bool(myActiveScene);
}

Scene& SceneManager::GetActiveScene()
{
	return *myActiveScene;
}

void SceneManager::UnloadActiveScene()
{
	Engine::GetObjectManager().ClearSceneObjects();
	myActiveScene.reset();
}

void SceneManager::LoadScene(const std::string& aPath)
{
	myActiveScene = std::make_unique<Scene>(mySceneLoader.GetScene(aPath));
	Engine::GetObjectManager().SetSceneObjects(&myActiveScene->gameObjects);
}

// 2024-09-08: This needs to be an external function due to how ThreadPool works currently
static Scene LoadSceneAsyncTask(const std::string& aPath, const SceneLoader& aLoader)
{
	return aLoader.GetScene(aPath);
}

void SceneManager::LoadSceneAsync(const std::string& aPath)
{
	myFuture = Engine::GetThreadPool().EnqueueFuture(LoadSceneAsyncTask, aPath, mySceneLoader);
	myIsLoadingScene = true;
}

void SceneManager::SaveScene(const std::string& aPath, const Scene& aScene, bool anAsBinary)
{
	mySceneLoader.SaveScene(aPath, aScene, anAsBinary);
}
