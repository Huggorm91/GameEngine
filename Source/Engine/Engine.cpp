#include "Engine.h"
#include "CrimsonUtilities\Math\Math.hpp"
#include "CrimsonUtilities\Time\Timer.h"
#include "CrimsonUtilities\Threadpool\ThreadPool.h"
#include "CrimsonUtilities\Input\InputMapper.h"
#include "CrimsonUtilities\Input\InputHandler.h"
#include "AssetManager\Managers\CollisionManager.h"

enum
{
	ENGINE_THREADS = 1,
	PHYSX_THREADS = 0,
	TOTAL_THREADS = ENGINE_THREADS + PHYSX_THREADS
};

Engine::Engine() :
	myThreadPool(nullptr),
	myInputMapper(nullptr),
	myInputHandler(nullptr),
	myCollisionManager(nullptr),
	myIsInitialized(false)
{}

Engine::~Engine()
{
	if (myIsInitialized)
	{
		delete myThreadPool;
		delete myInputMapper;
		delete myInputHandler;
		delete myCollisionManager;
	}
}

Engine& Engine::Get()
{
	static Engine instance;
	return instance;
}

void Engine::Init(HWND aHandle)
{
	auto& instance = Get();
	if (!instance.myIsInitialized)
	{
		instance.myThreadPool = new Crimson::ThreadPool(Crimson::Min(1, static_cast<int>(std::thread::hardware_concurrency()) - TOTAL_THREADS));
		instance.myInputMapper = new Crimson::InputMapper();
		instance.myInputMapper->Init(aHandle);
		instance.myInputHandler = new Crimson::InputHandler(*instance.myInputMapper);
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
	instance.myInputMapper->Update();
	instance.myCollisionManager->CheckCollisions();
	instance.myCollisionManager->EndFrame();
}

Crimson::ThreadPool& Engine::GetThreadPool()
{
	return *Get().myThreadPool;
}

Crimson::InputMapper& Engine::GetInputMapper()
{
	return *Get().myInputMapper;
}

CollisionManager& Engine::GetCollisionManager()
{
	return *Get().myCollisionManager;
}

bool Engine::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!Get().myIsInitialized)
	{
		return false;
	}
	return Get().myInputHandler->UpdateEvents(message, wParam, lParam);
}