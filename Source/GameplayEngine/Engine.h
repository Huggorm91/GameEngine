#pragma once
#include "CrimsonUtilities/Math/Vector2.hpp"

namespace Crimson
{
	class ThreadPool;
	class PostMaster;
	class InputMapper;
	class InputHandler;
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

	std::unique_ptr<Crimson::ThreadPool> myThreadPool;
	std::unique_ptr<Crimson::InputMapper> myInputMapper;
	std::unique_ptr<Crimson::InputHandler> myInputHandler;
	std::unique_ptr<Crimson::PostMaster> myPostMaster;
	std::unique_ptr<CollisionManager> myCollisionManager;

	bool myIsInitialized;
};