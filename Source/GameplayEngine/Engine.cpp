#include "Engine.h"
#include "CrimsonUtilities\Math\Math.hpp"
#include "CrimsonUtilities\Time\Timer.h"
#include "CrimsonUtilities\Threadpool\ThreadPool.h"
#include "CrimsonUtilities\Input\InputMapper.h"
#include "CrimsonUtilities\Input\InputHandler.h"
#include "CrimsonUtilities\PostMaster\PostMaster.h"
#include "AssetManager\Managers\CollisionManager.h"

enum
{
	ENGINE_THREADS = 1,
	PHYSX_THREADS = 0,
	TOTAL_THREADS = ENGINE_THREADS + PHYSX_THREADS
};

Engine::Engine() :
	myWindowHandle(NULL),
	myThreadPool(nullptr),
	myInputMapper(nullptr),
	myInputHandler(nullptr),
	myPostMaster(nullptr),
	myCollisionManager(nullptr),
	myIsInitialized(false)
{}

Engine::~Engine()
{
	if (myIsInitialized)
	{
		myIsInitialized = false;
		delete myThreadPool;
		delete myInputMapper;
		delete myInputHandler;
		delete myPostMaster;
		delete myCollisionManager;
	}
}

Engine& Engine::Get()
{
	static Engine instance;
	return instance;
}

void Engine::Init(HWND aHandle, const Crimson::Vector2i& aWindowSize)
{
	auto& instance = Get();
	if (!instance.myIsInitialized)
	{
		// Window
		instance.myWindowSize = aWindowSize;
		instance.myWindowHandle = aHandle;

		// Threads
		instance.myThreadPool = new Crimson::ThreadPool(Crimson::Max(1, static_cast<int>(std::thread::hardware_concurrency()) - TOTAL_THREADS));

		// Input
		instance.myInputMapper = new Crimson::InputMapper();
		instance.myInputMapper->Init(aHandle);
		instance.myInputHandler = new Crimson::InputHandler(*instance.myInputMapper);

		// Events
		instance.myPostMaster = new Crimson::PostMaster();

		// Collision
		instance.myCollisionManager = new CollisionManager();

		instance.myIsInitialized = true;
	}
}

void Engine::BeginFrame()
{
	Crimson::Timer::Update();
	Get().myInputMapper->Notify();
}

void Engine::EndFrame()
{
	auto& instance = Get();
	instance.myPostMaster->SendSavedMessages();
	instance.myCollisionManager->CheckCollisions();
	instance.myCollisionManager->EndFrame();
	instance.myInputMapper->Update();
}

bool Engine::IsValid()
{
	return Get().myIsInitialized;
}

Crimson::ThreadPool& Engine::GetThreadPool()
{
	return *Get().myThreadPool;
}

Crimson::InputMapper& Engine::GetInputMapper()
{
	return *Get().myInputMapper;
}

Crimson::PostMaster& Engine::GetPostMaster()
{
	return *Get().myPostMaster;
}

CollisionManager& Engine::GetCollisionManager()
{
	return *Get().myCollisionManager;
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
	if (!Get().myIsInitialized)
	{
		return false;
	}
	return Get().myInputHandler->UpdateEvents(message, wParam, lParam);
}