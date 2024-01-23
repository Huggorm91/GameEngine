#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "Math/Vector2.hpp"

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

	static void Init(HWND aHandle, const Crimson::Vector2i& aWindowSize);

	static void BeginFrame();
	static void EndFrame();

	static bool IsValid();

	static Crimson::ThreadPool& GetThreadPool();
	static Crimson::InputMapper& GetInputMapper();
	static Crimson::PostMaster& GetPostMaster();
	static CollisionManager& GetCollisionManager();

	static const Crimson::Vector2i& GetWindowSize();
	static HWND GetWindowHandle();

	static bool HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
private:
	Engine();
	static Engine& Get();

	Crimson::Vector2i myWindowSize;
	HWND myWindowHandle;

	Crimson::ThreadPool* myThreadPool;
	Crimson::InputMapper* myInputMapper;
	Crimson::InputHandler* myInputHandler;
	Crimson::PostMaster* myPostMaster;
	CollisionManager* myCollisionManager;

	bool myIsInitialized;
};