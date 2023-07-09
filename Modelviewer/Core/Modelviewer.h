#pragma once
#include <memory>

#include "ApplicationState.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "Logging/Logging.h"

#include "AssetManager/Assets/GameObject.h"
#include "GraphicsEngine/Camera/PerspectiveCamera.h"

class SplashWindow;

class ModelViewer
{
	HINSTANCE myModuleHandle = nullptr;
	HWND myMainWindowHandle = nullptr;

	SplashWindow* mySplashWindow = nullptr;

	ApplicationState myApplicationState;

	Logger myLogger;
	PerspectiveCamera myCamera;

	std::vector<GameObject> myGameObjects;

	ModelViewer() = default;

	void ShowSplashScreen();
	void HideSplashScreen() const;
	size_t LoadAllAssets();

	void LoadScene();

	void UpdateScene();

	void Init();
	void Update();

public:

	// Singleton Getter.
	static ModelViewer& Get() { static ModelViewer myInstance; return myInstance; }

	// Acceleration Getters for components.
	FORCEINLINE static ApplicationState& GetApplicationState() { return Get().myApplicationState; }
	FORCEINLINE static Logger& GetLogger() { return Get().myLogger; }

	bool Initialize(HINSTANCE aHInstance, SIZE aWindowSize, WNDPROC aWindowProcess, LPCWSTR aWindowTitle);
	int Run();
};
