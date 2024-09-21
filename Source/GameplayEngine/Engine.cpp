#include "GameplayEngine.pch.h"
#include "Engine.h"
#include "Logging\Logging.h"
#include "Logging\MainLogger.h"
#include "CrimsonUtilities\Time\Time.h"
#include "Threadpool\ThreadPool.h"
#include "Input\InputMapper.h"
#include "Input\InputHandler.h"
#include "PostMaster\PostMaster.h"
#include "Managers\CollisionManager.h"
#include "Managers\ObjectManager.h"
#include "Managers\SceneManager.h"
#include "Network\NetworkManager.h"

enum
{
	ENGINE_THREADS = 1,
	PHYSX_THREADS = 0,
	TOTAL_THREADS = ENGINE_THREADS + PHYSX_THREADS
};

Engine::Engine() :
	myWindowHandle(NULL),
	myIsInitialized(false)
{}

Engine::~Engine()
{
	myIsInitialized = false;
}

Engine& Engine::Get()
{
	static Engine instance;
	return instance;
}
#ifndef NETWORK_SERVER
void Engine::Init(HWND aHandle, const Crimson::Vector2i& aWindowSize, bool aShouldConnectToNetwork)
#else
void Engine::Init(GameServer* aServer)
#endif // !NETWORK_SERVER
{
	auto& instance = Get();
	if (!instance.myIsInitialized)
	{
#ifndef NETWORK_SERVER
		// Window
		instance.myWindowSize = aWindowSize;
		instance.myWindowHandle = aHandle;
#endif

		// Logging
		instance.myLogger = std::make_unique<MainLogger>();
		Logger::ourMainLogger = instance.myLogger.get();

		// Blackboard
		instance.myBlackboard = std::make_unique<Crimson::Blackboard<std::string>>();

		// Threads
		instance.myThreadPool = std::make_unique<ThreadPool>(Crimson::Max(1, static_cast<int>(std::thread::hardware_concurrency()) - TOTAL_THREADS));

#ifndef NETWORK_SERVER
		// Input
		instance.myInputMapper = std::make_unique<InputMapper>();
		instance.myInputMapper->Init(aHandle);
		instance.myInputHandler = std::make_unique<InputHandler>(*instance.myInputMapper);
#endif

		// Events
		instance.myPostMaster = std::make_unique<PostMaster>();

		// Managers
		instance.myCollisionManager = std::make_unique<CollisionManager>();
		instance.myObjectManager = std::make_unique<ObjectManager>();
		instance.mySceneManager = std::make_unique<SceneManager>();

		// Network
#ifndef NETWORK_SERVER
		if (aShouldConnectToNetwork)
		{
			instance.myNetworkManager = std::make_unique<NetworkManager>();
			instance.myNetworkManager->Init();
		}
#else
		instance.myNetworkManager = aServer;
#endif

		instance.myIsInitialized = true;
	}
}

void Engine::BeginFrame()
{
	Crimson::Time::Update();
	auto& instance = Get();
	if (!IsServer())
	{
		instance.myInputMapper->Notify();
	}

#ifndef NETWORK_SERVER
	if (instance.myNetworkManager)
	{
		instance.myNetworkManager->Update();
	}
#endif
}

void Engine::EndFrame()
{
	auto& instance = Get();
	instance.myPostMaster->SendSavedMessages();
	instance.myCollisionManager->CheckCollisions();
	instance.myCollisionManager->EndFrame();
	instance.myObjectManager->EndOfFrame();
	if (!IsServer())
	{
		instance.myInputMapper->Update();
	}
}

bool Engine::IsValid()
{
	return Get().myIsInitialized;
}

bool Engine::IsNetworkingEnabled()
{
	return bool(Get().myNetworkManager);
}

Crimson::Blackboard<std::string>& Engine::GetBlackboard()
{
	return *Get().myBlackboard;
}

MainLogger& Engine::GetLogger()
{
	return *Get().myLogger;
}

ThreadPool& Engine::GetThreadPool()
{
	return *Get().myThreadPool;
}

InputMapper& Engine::GetInputMapper()
{
	return *Get().myInputMapper;
}

PostMaster& Engine::GetPostMaster()
{
	return *Get().myPostMaster;
}

CollisionManager& Engine::GetCollisionManager()
{
	return *Get().myCollisionManager;
}

ObjectManager& Engine::GetObjectManager()
{
	return *Get().myObjectManager;
}

SceneManager& Engine::GetSceneManager()
{
	return *Get().mySceneManager;
}

#ifndef NETWORK_SERVER
NetworkManager& Engine::GetNetworkManager()
#else
GameServer& Engine::GetNetworkManager()
#endif
{
	return *Get().myNetworkManager;
}

const Crimson::Vector2i& Engine::GetWindowSize()
{
	return Get().myWindowSize;
}

HWND Engine::GetWindowHandle()
{
	return Get().myWindowHandle;
}

bool Engine::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& instance = Get();
	if (!instance.myIsInitialized)
	{
		return false;
	}
	return instance.myInputHandler->UpdateEvents(message, wParam, lParam);
}