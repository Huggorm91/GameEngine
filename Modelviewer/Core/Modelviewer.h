#pragma once
#include <memory>

#include "ApplicationState.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Camera/PerspectiveCamera.h"
#include "Logging/Logging.h"


class SplashWindow;

#ifndef _RETAIL
#include "Editor/ImguiManager.h"
#include "Commands/EditCommand.h"

class ModelViewer: public Crimson::InputObserver
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
	FORCEINLINE static ImguiManager& GetImguiManager() { return Get().myImguiManager; }
	void SetDropFile(HDROP aHandle);

	void AddCommand(const std::shared_ptr<EditCommand>& aCommand);

	std::shared_ptr<GameObject>& AddGameObject(bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(const std::shared_ptr<GameObject>& anObject, bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(GameObject&& anObject, bool aAddToUndo = true);

	std::shared_ptr<GameObject> GetGameObject(unsigned anID);
	std::shared_ptr<GameObject> GetGameObject(const Crimson::Vector2f& aScreenPosition);
#else
	GameObject& AddGameObject();
	GameObject& AddGameObject(const GameObject& anObject);
	GameObject& AddGameObject(GameObject&& anObject);

	GameObject* GetGameObject(unsigned anID);
	GameObject* GetGameObject(const Crimson::Vector2f& aScreenPosition);
#endif // _RETAIL

	bool RemoveGameObject(unsigned anID);

	void SaveState() const;

	void SaveScene(const std::string& aPath) const;
	void LoadScene(const std::string& aPath);

	static inline const char* GetSceneExtension(){ return ".lvl"; }
	static inline const char* GetScenePath(){ return "Content\\Scenes\\"; }

#ifndef _RETAIL
	void ReceiveEvent(Crimson::eInputEvent, Crimson::eKey) override;
	void ReceiveEvent(Crimson::eInputAction, float) override;
#endif // _RETAIL

private:
#ifndef _RETAIL
	friend class ImguiManager;
	friend class EditCommand;

	GraphicsEngine::DebugMode myDebugMode;
	GraphicsEngine::LightMode myLightMode;
	GraphicsEngine::RenderMode myRenderMode;

	ImguiManager myImguiManager;

	std::vector<std::shared_ptr<EditCommand>> myRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myUndoCommands;
#endif // _RETAIL
	HINSTANCE myModuleHandle{ nullptr };
	HWND myMainWindowHandle{ nullptr };

	SplashWindow* mySplashWindow{ nullptr };

	std::string mySettingsPath{"Settings/mw_settings.json"};
	std::string myLoadedScene;
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
	void UndoCommand();
	void RedoCommand();
#endif // _RETAIL
};
