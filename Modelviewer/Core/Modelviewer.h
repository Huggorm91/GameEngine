#pragma once
#include <memory>
#include <shellapi.h>

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

#ifndef _RETAIL
	void SetDropFile(HDROP aHandle);

	void AddCommand(const std::shared_ptr<EditCommand>& aCommand);

	std::shared_ptr<GameObject>& AddGameObject(bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(const std::shared_ptr<GameObject>& anObject, bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(GameObject&& anObject, bool aAddToUndo = true);

	std::shared_ptr<GameObject> GetGameObject(unsigned anID);
	std::shared_ptr<GameObject> GetGameObject(const CommonUtilities::Vector2f& aScreenPosition);
#else
	GameObject& AddGameObject();
	GameObject& AddGameObject(const GameObject& anObject);
	GameObject& AddGameObject(GameObject&& anObject);

	GameObject* GetGameObject(unsigned anID);
	GameObject* GetGameObject(const CommonUtilities::Vector2f& aScreenPosition);
#endif // _RETAIL

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
	bool myIsShowingDragFilePopUp;

	GraphicsEngine::DebugMode myDebugMode;
	GraphicsEngine::LightMode myLightMode;
	GraphicsEngine::RenderMode myRenderMode;

	HDROP myDropfile;
	unsigned myDropFileCount;
	unsigned myDropFileSelection;
	CommonUtilities::Vector2i myDropLocation;

	ComponentType mySelectedComponentType;
	std::weak_ptr<GameObject> mySelectedObject;
	const std::string* mySelectedPrefabName;
	std::string mySelectedPath;
	Prefab myEditPrefab;
	GameObject myNewObject;
	std::unordered_map<std::string, unsigned> myImguiNameCounts;

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

#ifndef _RETAIL
	std::unordered_map<unsigned, std::shared_ptr<GameObject>> myGameObjects;
#else
	std::unordered_map<unsigned, GameObject> myGameObjects;
#endif // _RETAIL

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
	void CreateDropFilePopUp();

	void CreateSelectedObjectWindow();
	void CreateSceneContentWindow();

	void CreatePrefabWindow();
	void CreateNewObjectWindow();

	void UndoCommand();
	void RedoCommand();

	std::string GetDropFilePath(unsigned anIndex);
#endif // _RETAIL
};
