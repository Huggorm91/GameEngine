#pragma once
#include <memory>

#include "ApplicationState.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "Logging/Logging.h"

#include "AssetManager/Assets/GameObject.h"
#include "GraphicsEngine/Camera/PerspectiveCamera.h"

class SplashWindow;

#ifndef _RETAIL
#include <InputHandler.h>
class ModelViewer: public CommonUtilities::InputObserver
{
#else
class ModelViewer
{
#endif // _RETAIL
public:
// Singleton Getter.
	static ModelViewer& Get() {
		static ModelViewer myInstance; return myInstance;
	}

// Acceleration Getters for components.
	FORCEINLINE static ApplicationState& GetApplicationState() {
		return Get().myApplicationState;
	}
	FORCEINLINE static Logger& GetLogger() {
		return Get().myLogger;
	}

	bool Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess);
	int Run();

	void SaveState() const;

#ifndef _RETAIL
	void ReceiveEvent(CommonUtilities::eInputEvent, CommonUtilities::eKey) override;
	void ReceiveEvent(CommonUtilities::eInputAction, float) override{}
#endif // _RETAIL

private:
#ifndef _RETAIL
	GraphicsEngine::DebugMode myDebugMode;
	GraphicsEngine::LightMode myLightMode;
	GraphicsEngine::RenderMode myRenderMode;
	int mySelectedIndex;
#endif // _RETAIL
	HINSTANCE myModuleHandle{ nullptr };
	HWND myMainWindowHandle{ nullptr };

	SplashWindow* mySplashWindow{ nullptr };

	std::string mySettingsPath{"Settings/mw_settings.json"};
	ApplicationState myApplicationState;

	Logger myLogger;
	PerspectiveCamera myCamera;

	std::vector<GameObject> myGameObjects;
	// std::unordered_map<unsigned, GameObject> myGameObjects;

	ModelViewer();

	void ShowSplashScreen();
	void HideSplashScreen() const;
	size_t LoadAllAssets();

	void SaveScene(const std::string& aPath);
	void LoadScene(const std::string& aPath);
	void LoadState();

	void UpdateScene();

	void Init();
	void Update();

#ifndef _RETAIL
	void InitImgui();
	void UpdateImgui();
	void RenderImgui();

	void CreatePreferenceWindow();
#endif // _RETAIL
};
