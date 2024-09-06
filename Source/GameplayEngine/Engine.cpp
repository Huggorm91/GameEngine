#include "GameplayEngine.pch.h"
#include "Engine.h"
#include "CrimsonUtilities\Time\Timer.h"
#include "Threadpool\ThreadPool.h"
#include "Input\InputMapper.h"
#include "Input\InputHandler.h"
#include "PostMaster\PostMaster.h"
#include "Managers\CollisionManager.h"

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

void Engine::Init(HWND aHandle, const Crimson::Vector2i& aWindowSize)
{
	auto& instance = Get();
	if (!instance.myIsInitialized)
	{
		// Window
		instance.myWindowSize = aWindowSize;
		instance.myWindowHandle = aHandle;

		// Threads
		instance.myThreadPool = std::make_unique<Crimson::ThreadPool>(Crimson::Max(1, static_cast<int>(std::thread::hardware_concurrency()) - TOTAL_THREADS));

		// Input
		instance.myInputMapper = std::make_unique<Crimson::InputMapper>();
		instance.myInputMapper->Init(aHandle);
		instance.myInputHandler = std::make_unique<Crimson::InputHandler>(*instance.myInputMapper);

		// Events
		instance.myPostMaster = std::make_unique<Crimson::PostMaster>();

		// Collision
		instance.myCollisionManager = std::make_unique<CollisionManager>();

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
	auto& instance = Get();
	if (!instance.myIsInitialized)
	{
		return false;
	}
	return instance.myInputHandler->UpdateEvents(message, wParam, lParam);
}