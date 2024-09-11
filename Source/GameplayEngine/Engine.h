#pragma once
#include "CrimsonUtilities/Math/Vector2.hpp"
#include "CrimsonUtilities/Container/Blackboard.hpp"

class ThreadPool;
class PostMaster;
class InputMapper;
class InputHandler;
class CollisionManager;
class ObjectManager;
class SceneManager;
class MainLogger;
class NetworkManager;

class Engine
{
public:
	~Engine();

	static void Init(HWND aHandle, const Crimson::Vector2i& aWindowSize, bool aShouldConnectToNetwork);

	static void BeginFrame();
	static void EndFrame();

	static bool IsValid();
	static bool IsNetworkingEnabled();

	static Crimson::Blackboard<std::string>& GetBlackboard();
	static MainLogger& GetLogger();
	static ThreadPool& GetThreadPool();
	static InputMapper& GetInputMapper();
	static PostMaster& GetPostMaster();
	static CollisionManager& GetCollisionManager();
	static ObjectManager& GetObjectManager();
	static SceneManager& GetSceneManager();
	static NetworkManager& GetNetworkManager();

	static const Crimson::Vector2i& GetWindowSize();
	static HWND GetWindowHandle();

	static bool HandleInput(UINT message, WPARAM wParam, LPARAM lParam);

private:
	Engine();
	static Engine& Get();

	Crimson::Vector2i myWindowSize;
	HWND myWindowHandle;

	std::unique_ptr<Crimson::Blackboard<std::string>> myBlackboard;
	std::unique_ptr<MainLogger> myLogger;
	std::unique_ptr<ThreadPool> myThreadPool;
	std::unique_ptr<InputMapper> myInputMapper;
	std::unique_ptr<InputHandler> myInputHandler;
	std::unique_ptr<PostMaster> myPostMaster;
	std::unique_ptr<CollisionManager> myCollisionManager;
	std::unique_ptr<ObjectManager> myObjectManager;
	std::unique_ptr<SceneManager> mySceneManager;
	std::unique_ptr<NetworkManager> myNetworkManager;

	bool myIsInitialized;
};