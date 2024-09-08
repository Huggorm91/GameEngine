#pragma once
#include <memory>
#include "Core/ApplicationState.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GameplayEngine/Scene/Scene.h"
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

class ModelViewer : public InputObserver
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
		return Get().myMessageHandler;
	}

	bool Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess);
	int Run();
	void Shutdown();

	FORCEINLINE static ImguiManager& GetImguiManager()
	{
		return Get().myImguiManager;
	}
	void SetDropFile(HDROP aHandle);

	void SetPlayMode(bool aState);
	void SetIsSceneActive(bool aState);

	void RestoreDebugSettings() const;

	void ActivateImGuiEditor();
	void ActivateSkeletonEditor();

	void SetCameraSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void AddCommand(const std::shared_ptr<EditCommand>& aCommand);

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
	Network::MessageHandler myMessageHandler;

	std::vector<std::shared_ptr<EditCommand>> myRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myUndoCommands;

	std::vector<std::shared_ptr<EditCommand>> myPlayModeRedoCommands;
	std::vector<std::shared_ptr<EditCommand>> myPlayModeUndoCommands;

	HINSTANCE myModuleHandle;
	HWND myMainWindowHandle;
	SplashWindow* mySplashWindow;

	std::string mySceneName;
	const std::string mySettingsPath;
	ApplicationState myApplicationState;

	Logger myLogger;
	GameObject myCamera;

	Scene myPlayModeScene;
	std::vector<UUIDv4::UUID> myObjectOrder;
	std::unordered_map<UUIDv4::UUID, std::shared_ptr<GameObject>> myGameobjects;
	std::unordered_map<UUIDv4::UUID, std::shared_ptr<GameObject>> myPlayModePointers;

	ModelViewer();

	void SetKeyBinds();

	void HandleCrash(const std::exception& anException, bool aTrySavingScene);

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
