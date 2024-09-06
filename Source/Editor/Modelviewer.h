#pragma once
#include <memory>
#include "Core/ApplicationState.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "AssetManager/Managers/SceneManager.h"
#include "Logging/Logging.h"
#include "Editor/SkeletonEditor.h"
#include "Editor/ImguiManager.h"
#include "Commands/EditCommand.h"
#include "NetworkClient/MessageHandler.h"

class SplashWindow;
class GameObject;
class ScriptGraphEditor;
class ScriptGraph;
struct ScriptGraphEditorSettings;
struct ScriptGraphEditorState;

class ModelViewer : public Crimson::InputObserver
{
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
	FORCEINLINE static Network::MessageHandler& GetMessageHandler() {
		return *Get().myMessageHandler;
	}

	bool Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess, HICON anIcon);
	int Run();
	void Shutdown();

	FORCEINLINE static ImguiManager& GetImguiManager()
	{
		return Get().myImguiManager;
	}
	void SetDropFile(HDROP aHandle);

	void SetPlayMode(bool aState);
	void SetIsSceneActive(bool aState);

	void RestoreDebugSettings();

	void ActivateImGuiEditor();
	void ActivateSkeletonEditor();

	void SetCameraSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void AddCommand(const std::shared_ptr<EditCommand>& aCommand);

	std::shared_ptr<GameObject>& AddGameObject(bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(const std::shared_ptr<GameObject>& anObject, bool aAddToUndo = true);
	std::shared_ptr<GameObject>& AddGameObject(GameObject&& anObject, bool aAddToUndo = true);

	std::shared_ptr<GameObject> GetGameObject(const UUIDv4::UUID& anID);
	std::shared_ptr<GameObject> GetGameObject(const Crimson::Vector2f& aScreenPosition);

	bool RemoveGameObject(const UUIDv4::UUID& anID);

	void SaveState() const;

	void SaveScene(const std::string& aPath, bool aAsBinary);
	void LoadScene(const std::string& aPath);

	void ReceiveEvent(Crimson::eInputEvent, Crimson::eKey) override;
	void ReceiveEvent(Crimson::eInputAction, float) override;

private:
	friend class ImguiManager;
	friend class EditCommand;

	bool mySceneIsEdited;
	bool myIsInPlayMode;
	bool myIsMaximized;
	bool myIsMovingCamera;
	bool myIsSceneActive;

	GraphicsEngine::DebugMode myDebugMode;
	GraphicsEngine::LightMode myLightMode;
	GraphicsEngine::RenderMode myRenderMode;

	std::shared_ptr<ScriptGraphEditor> myScriptGraphEditor;
	std::shared_ptr<ScriptGraph>myScriptGraph;
	std::shared_ptr<ScriptGraphEditorSettings> myScriptGraphEditorSettings;
	std::shared_ptr<ScriptGraphEditorState> myScriptGraphEditorState;
	
	ImguiManager myImguiManager;
	SkeletonEditor mySkeletonEditor;

	std::vector<std::shared_ptr<EditCommand>> myRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myUndoCommands;

	std::vector<std::shared_ptr<EditCommand>> myPlayModeRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myPlayModeUndoCommands;

	HINSTANCE myModuleHandle;
	HWND myMainWindowHandle;

	SplashWindow* mySplashWindow;
	std::unique_ptr<Network::MessageHandler> myMessageHandler;

	const std::string mySettingsPath;
	ApplicationState myApplicationState;

	Logger myLogger;
	GameObject myCamera;

	EditorScene myScene;
	Scene myPlayScene;
	std::unordered_map<UUIDv4::UUID, std::shared_ptr<GameObject>> myPlayScenePointers;

	ModelViewer();

	void HandleCrash(const std::exception& anException);

	void ShowSplashScreen();
	void HideSplashScreen() const;

	void LoadState();

	void UpdateScene();

	void Init();
	void Update();

	void HandleNetmessages();

	void UndoCommand();
	void RedoCommand();
};
