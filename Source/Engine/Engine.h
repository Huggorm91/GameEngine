#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "Windows.h"

namespace Crimson
{
	class ThreadPool;
	class InputMapper;
	class InputHandler;
	class PostMaster;
}
class CollisionManager;

class Engine
{
public:
	~Engine();

	static void Init(HWND aHandle);

	static void BeginFrame();
	static void EndFrame();

	static bool IsValid();

	static Crimson::ThreadPool& GetThreadPool();
	static Crimson::InputMapper& GetInputMapper();
	static Crimson::PostMaster& GetPostMaster();
	static CollisionManager& GetCollisionManager();

	static HWND GetWindowHandle();

	static bool HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
private:
	Engine();
	static Engine& Get();

	HWND myWindowHandle;

	Crimson::ThreadPool* myThreadPool;
	Crimson::InputMapper* myInputMapper;
	Crimson::InputHandler* myInputHandler;
	Crimson::PostMaster* myPostMaster;
	CollisionManager* myCollisionManager;

	bool myIsInitialized;
};