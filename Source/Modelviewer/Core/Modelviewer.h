#pragma once
#include <memory>
#include "ApplicationState.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "AssetManager/Managers/SceneManager.h"
#include "Logging/Logging.h"
#include "ScriptGraphEditor/ScriptGraphEditor.h"
#include "PostMaster/Observer.h"

// TODO: 
// Make all saved filepaths relative to the assettypes folder
// Serialize components
// Deserialize components
//
class SplashWindow;
class GameObject;

#ifndef _RETAIL
#include "Editor/ImguiManager.h"
#include "Commands/EditCommand.h"

class ModelViewer : public Crimson::InputObserver, public Crimson::Observer
{
#else
class ModelViewer
{
#endif // _RETAIL
public:
	// Singleton Getter.
	static ModelViewer& Get()
	{
		static ModelViewer myInstance; return myInstance;
	}

	// Acceleration Getters for components.
	FORCEINLINE static ApplicationState& GetApplicationState()
	{
		return Get().myApplicationState;
	}
	FORCEINLINE static Logger& GetLogger()
	{
		return Get().myLogger;
	}
	FORCEINLINE static GameObject& GetCamera()
	{
		return Get().myCamera;
	}
	FORCEINLINE static std::vector<GameObjectID>& GetCreatedObjects()
	{
		return Get().myObjectsCreatedByScript;
	}

	bool Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess);
	int Run();

#ifndef _RETAIL
	FORCEINLINE static ImguiManager& GetImguiManager()
	{
		return Get().myImguiManager;
	}
	void SetDropFile(HDROP aHandle);

	void SetPlayMode(bool aState);

	void AddCommand(const std::shared_ptr<EditCommand>& aCommand);

	std::shared_ptr<GameObject>& AddGameObject(bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(const std::shared_ptr<GameObject>& anObject, bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(GameObject&& anObject, bool aAddToUndo = true);

	std::shared_ptr<GameObject> GetGameObject(GameObjectID anID);
	std::shared_ptr<GameObject> GetGameObject(const Crimson::Vector2f& aScreenPosition);
#else
	GameObject& AddGameObject();
	GameObject& AddGameObject(const GameObject& anObject);
	GameObject& AddGameObject(GameObject&& anObject);

	GameObject* GetGameObject(GameObjectID anID);
	GameObject* GetGameObject(const Crimson::Vector2f& aScreenPosition);
#endif // _RETAIL

	bool RemoveGameObject(GameObjectID anID);

	void SaveState() const;

	void SaveScene(const std::string& aPath, bool aAsBinary);
	void LoadScene(const std::string& aPath);

#ifndef _RETAIL
	void ReceiveEvent(Crimson::eInputEvent, Crimson::eKey) override;
	void ReceiveEvent(Crimson::eInputAction, float) override;
#endif // _RETAIL

	void RecieveMessage(const Crimson::Message& aMessage) override;

private:
	bool myIsAcceptingInput;

#ifndef _RETAIL
	friend class ImguiManager;
	friend class EditCommand;

	bool mySceneIsEdited;
	bool myIsInPlayMode;
	bool myIsMaximized;

	GraphicsEngine::DebugMode myDebugMode;
	GraphicsEngine::LightMode myLightMode;
	GraphicsEngine::RenderMode myRenderMode;

	ImguiManager myImguiManager;

	std::vector<std::shared_ptr<EditCommand>> myRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myUndoCommands;

	std::vector<std::shared_ptr<EditCommand>> myPlayModeRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myPlayModeUndoCommands;
#endif // _RETAIL
	HINSTANCE myModuleHandle{ nullptr };
	HWND myMainWindowHandle{ nullptr };

	SplashWindow* mySplashWindow{ nullptr };

	const std::string mySettingsPath{ "Settings/mw_settings.json" };
	ApplicationState myApplicationState;

	Logger myLogger;
	GameObject myCamera;

	std::vector<GameObjectID> myObjectsCreatedByScript;

#ifndef _RETAIL
	EditorScene myScene;
	Scene myPlayScene;
	std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> myPlayScenePointers;

	ScriptGraphEditor myScriptGraphEditor;
#else
	Scene myScene;
#endif // _RETAIL

	ModelViewer();

	void SetKeyBinds();

	void HandleCrash(const std::exception& anException);

	void ShowSplashScreen();
	void HideSplashScreen() const;

	void LoadState();

	void UpdateScene();

	void Init();
	void Update();

#ifndef _RETAIL
	void UndoCommand();
	void RedoCommand();
#endif // _RETAIL
};
