#pragma once
#include <memory>

#include "ApplicationState.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "Logging/Logging.h"

#include "AssetManager/Assets/Prefab.h"
#include "GraphicsEngine/Camera/PerspectiveCamera.h"

#include "Commands/EditCommand.h"

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

	GameObject& AddGameObject();
	GameObject& AddGameObject(const GameObject& anObject);
	GameObject& AddGameObject(GameObject&& anObject);

	GameObject* GetGameObject(unsigned anID);
	GameObject* GetGameObject(const CommonUtilities::Vector4f& anID);

	bool RemoveGameObject(unsigned anID);

	void SaveState() const;
	void SaveScene(const std::string& aPath) const;
	void LoadScene(const std::string& aPath);

#ifndef _RETAIL
	void ReceiveEvent(CommonUtilities::eInputEvent, CommonUtilities::eKey) override;
	void ReceiveEvent(CommonUtilities::eInputAction, float) override;
#endif // _RETAIL

private:
#ifndef _RETAIL
	friend class EditCommand;

	bool myIsEditingPrefab;
	bool myIsShowingPrefabWindow;
	bool myIsShowingNewObjectWindow;

	GraphicsEngine::DebugMode myDebugMode;
	GraphicsEngine::LightMode myLightMode;
	GraphicsEngine::RenderMode myRenderMode;

	ComponentType mySelectedComponentType;
	GameObject* mySelectedObject; 
	const std::string* mySelectedPrefabName;
	std::string mySelectedPath;
	Prefab myEditPrefab;
	GameObject myNewObject;
	std::unordered_map<std::string, unsigned> myImguiNameCounts;

	std::vector<std::shared_ptr<EditCommand>> myIncommingCommands;
	std::vector<std::shared_ptr<EditCommand>> myRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myUndoCommands;
#endif // _RETAIL
	HINSTANCE myModuleHandle{ nullptr };
	HWND myMainWindowHandle{ nullptr };

	SplashWindow* mySplashWindow{ nullptr };

	std::string mySettingsPath{"Settings/mw_settings.json"};
	ApplicationState myApplicationState;

	Logger myLogger;
	PerspectiveCamera myCamera;

	std::unordered_map<unsigned, GameObject> myGameObjects;

	ModelViewer();

	void ShowSplashScreen();
	void HideSplashScreen() const;
	size_t LoadAllAssets();

	void LoadState();

	void UpdateScene();

	void Init();
	void Update();

#ifndef _RETAIL
	void InitImgui();
	void UpdateImgui();
	void RenderImgui();

	void CreatePreferenceWindow();
	void CreateSelectedObjectWindow();
	void CreateSceneContentWindow();

	void CreatePrefabWindow();
	void CreateNewObjectWindow();

	void AddCommand(const std::shared_ptr<EditCommand>& aCommand);
	void Undo();
	void Redo();
#endif // _RETAIL
};
