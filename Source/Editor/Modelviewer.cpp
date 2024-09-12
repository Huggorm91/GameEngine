#include "Editor.pch.h"
#include "Modelviewer.h"
#include "Modelviewer/resource.h"
#include "Windows/SplashWindow.h"
#include "Commands/EditCmd_AddGameobject.h"
#include "Commands/EditCmd_RemoveGameobject.h"

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "AssetManager/Assets/Binary.h"
#include "AssetManager/Assets/Components/Camera/PerspectiveCameraComponent.h"
#include "AssetManager/Assets/Components/Camera/EditorCameraControllerComponent.h"

#include "NetworkShared/MessageFunctions.h"

#include "CrimsonUtilities/Time/Time.h"
#include "CrimsonUtilities/Json/jsonCpp/json.h"

#include "GameplayEngine/Input/InputMapper.h"
#include "GameplayEngine/Managers/SceneManager.h"
#include "GameplayEngine/Managers/ObjectManager.h"
#include "GameplayEngine/Network/NetworkManager.h"

#include "Logging/MainLogger.h"

ModelViewer::ModelViewer() :
	myModuleHandle(nullptr),
	myMainWindowHandle(nullptr),
	mySplashWindow(nullptr),
	mySettingsPath("Settings/mw_settings.json"),
	myIsMovingCamera(false),
	myDebugMode(GraphicsEngine::DebugMode::Default),
	myLightMode(GraphicsEngine::LightMode::Default),
	myRenderMode(GraphicsEngine::RenderMode::Mesh),
	myIsInPlayMode(false),
	myIsMaximized(false),
	mySceneIsEdited(false),
	myIsSceneActive(true)
{}

void ModelViewer::SetKeyBinds()
{
	auto& input = Engine::GetInputMapper();
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F1);

	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F4);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F5);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F6);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F7);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F8);

	input.BindAction(Crimson::eInputAction::Undo, Crimson::KeyBind{ Crimson::eKey::Z, Crimson::eKey::Ctrl });
	input.BindAction(Crimson::eInputAction::Redo, Crimson::KeyBind{ Crimson::eKey::Y, Crimson::eKey::Ctrl });
	input.Attach(this, Crimson::eInputAction::Undo);
	input.Attach(this, Crimson::eInputAction::Redo);
}

void ModelViewer::HandleCrash(const std::exception& anException, bool aTrySavingScene)
{
	// Center console and bring it to the front
	{
		HWND consoleWindow = GetConsoleWindow();
		RECT consolePos;
		GetWindowRect(consoleWindow, &consolePos);
		consolePos.right = consolePos.right - consolePos.left;
		consolePos.bottom = consolePos.bottom - consolePos.top;

		RECT windowRect{};
		SystemParametersInfo(SPI_GETWORKAREA, 0, &windowRect, 0);

		windowRect.left = static_cast<LONG>((windowRect.right * 0.5f) - (consolePos.right * 0.5f));
		windowRect.top = static_cast<LONG>((windowRect.bottom * 0.5f) - (consolePos.bottom * 0.5f));

		ShowWindow(myMainWindowHandle, SW_HIDE);
		SetWindowPos(consoleWindow, HWND_TOP, windowRect.left, windowRect.top, consolePos.right, consolePos.bottom, 0);
		SetForegroundWindow(consoleWindow);
	}

	// Log crash
	myLogger.Err("Program has crashed!");
	myLogger.Warn("Writing exception to log file!");
	myLogger.LogException(anException);
	Engine::GetLogger().PrintHistoryToFile("EditorLogs\\" + Crimson::FileNameTimestamp() + "_Log.txt");

	if (aTrySavingScene)
	{
		// Save current scene if possible
		std::string saveName = "Bin\\Crashdump\\" + Crimson::FileNameTimestamp() + "_" + mySceneName;
		try
		{
			SaveScene("..\\" + saveName, false);
			myLogger.Succ("Saved current scene to: " + saveName);
		}
		catch (...)
		{
			myLogger.Err("Failed to save current scene to: " + saveName);
		}
	}	

	// Leave console up to let user read information
	system("PAUSE");
}

bool ModelViewer::Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess)
{
	myModuleHandle = aHInstance;
	LoadState();

	constexpr LPCWSTR windowClassName = L"CrimsonEngine_MainWindow";

	// First we create our Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = aWindowProcess;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = windowClassName;
	windowClass.hIcon = LoadIcon(aHInstance, MAKEINTRESOURCE(IDI_MODELVIEWER_ICON));
	RegisterClass(&windowClass);

	std::wstring stdTitle{ Crimson::ToWString(myApplicationState.windowTitle) };
	LPCWSTR title{ stdTitle.c_str() };

	DWORD flags;
	if (myApplicationState.startMaximized)
	{
		flags = WS_OVERLAPPEDWINDOW | WS_POPUP | WS_MAXIMIZE;
	}
	else
	{
		flags = WS_OVERLAPPEDWINDOW | WS_POPUP;
	}
	myIsMaximized = myApplicationState.startMaximized;

	// Get center of screen
	RECT windowRect{};
	SystemParametersInfo(SPI_GETWORKAREA, 0, &windowRect, 0);

	if (myApplicationState.windowSize == Crimson::Vector2i::Null)
	{
		myApplicationState.windowSize = { 1920, 1080 };
	}

	windowRect.left = static_cast<LONG>((windowRect.right * 0.5f) - (myApplicationState.windowSize.x * 0.5f));
	windowRect.top = static_cast<LONG>((windowRect.bottom * 0.5f) - (myApplicationState.windowSize.y * 0.5f));
	windowRect.right = myApplicationState.windowSize.x;
	windowRect.bottom = myApplicationState.windowSize.y;

	myMainWindowHandle = CreateWindow(
		windowClassName,	// Classname
		title,				// Window Title
		flags,				// Flags / Style
		windowRect.left,	// x coord
		windowRect.top,		// y coord
		windowRect.right,	// width
		windowRect.bottom,	// height
		nullptr,			// Parent hwnd
		nullptr,			// hMenu
		nullptr,			// hInstance
		nullptr				// lpParam
	);

	ShowSplashScreen();

#ifndef _DEBUG
	try
	{
#endif // _DEBUG
	Engine::Init(myMainWindowHandle, myApplicationState.windowSize, false);

	myLogger = Logger::Create("ModelViewer");
	myLogger.Log(Crimson::GetAppPath());
#ifdef _RETAIL
	GraphicsEngine::Get().Initialize(myMainWindowHandle, false);
#else
	GraphicsEngine::Get().Initialize(myMainWindowHandle, true);
#endif // _RETAIL

	AssetManager::Init();
	AssetManager::GeneratePrimitives();

	AssetManager::PreLoadAssets();
	myImguiManager.Init(false);

	SetKeyBinds();

	constexpr float fov = 90.f;
	constexpr float nearPlane = 1.f;
	constexpr float farPlane = 10000.f;

	myCamera.SetPosition({ 0.f, 200.f, 0.f });
	myCamera.AddComponent(PerspectiveCameraComponent(fov, nearPlane, farPlane));
	myCamera.AddComponent(EditorCameraControllerComponent(myApplicationState.cameraSpeed, myApplicationState.cameraMouseSensitivity));

	mySkeletonEditor.Init(fov, nearPlane, farPlane, myApplicationState.cameraSpeed, myApplicationState.cameraMouseSensitivity);

	MuninGraph::Get().Initialize();
	myScriptGraphEditorSettings = std::make_shared<ScriptGraphEditorSettings>(RHI::Device);
	myScriptGraphEditorState = std::make_shared<ScriptGraphEditorState>();
	myScriptGraph = std::make_shared<ScriptGraph>();
	myScriptGraphEditor = std::make_shared<ScriptGraphEditor>(myScriptGraphEditorSettings.get(), myScriptGraphEditorState.get(), myScriptGraph.get());

	DragAcceptFiles(myMainWindowHandle, TRUE);

#ifndef _DEBUG
	}
	catch (const std::exception& anException)
	{
		HandleCrash(anException, false);
		Shutdown();
	}
	catch (...)
	{
		HandleCrash(std::invalid_argument("Caught unknown Error!"), false);
		Shutdown();
	}
#endif // _DEBUG
	HideSplashScreen();

	return true;
}

int ModelViewer::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Init();
	Crimson::Time::Init();

	bool isRunning = true;
	while (isRunning)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				isRunning = false;
			}
		}

#ifndef _DEBUG
		try
		{
#endif // _DEBUG

			Update();

#ifndef _DEBUG
		}
		catch (const std::exception& anException)
		{
			isRunning = false;
			HandleCrash(anException, true);
		}
		catch (...)
		{
			isRunning = false;
			HandleCrash(std::invalid_argument("Caught unknown Error!"), true);
		}
#endif // _DEBUG
	}

	Shutdown();

	return 0;
}

void ModelViewer::Shutdown()
{
	myImguiManager.Release();
}

void ModelViewer::SetDropFile(HDROP aHandle)
{
	try
	{
		myImguiManager.SetDropFile(aHandle);
	}
	catch (const std::exception& anException)
	{
		myLogger.Err("Failed to accept dropped files!");
		myLogger.LogException(anException);
		myImguiManager.ReleaseDropFile();
	}
}

void ModelViewer::SetPlayMode(bool aState)
{
	myIsInPlayMode = aState;
	myPlayModePointers.clear();
	myPlayModeRedoCommands.clear();
	myPlayModeUndoCommands.clear();

	if (myIsInPlayMode)
	{
		std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
		for (auto& [id, object] : myGameobjects)
		{
			GameObject copy = *object;
			copy.CopyUuidOf(*object);

			if (object->HasParent())
			{
				childlist.emplace(id, object->GetParent()->GetUUID());
			}
			
			auto pointer = std::shared_ptr<GameObject>(&myPlayModeScene.gameObjects.emplace(id, std::move(copy)).first->second, [](GameObject*) {});
			myPlayModePointers.emplace(id, pointer);
		}

		for (auto& [childID, parentID] : childlist)
		{
			myPlayModePointers.at(parentID)->AddChild(myPlayModePointers.at(childID).get());
		}

		Engine::GetObjectManager().SetTemporaryObjects(&myPlayModeScene.gameObjects);
		myImguiManager.SetActiveObjects(&myPlayModePointers);
	}
	else
	{
		myImguiManager.SetActiveObjects(&myGameobjects);		
	}
}

void ModelViewer::SetIsSceneActive(bool aState)
{
	myIsSceneActive = aState;
	myCamera.SetActive(aState);
	GraphicsEngine::Get().SetDrawGridLines(aState);
}

void ModelViewer::RestoreDebugSettings() const
{
	auto& engine = GraphicsEngine::Get();
	engine.SetDebugMode(myDebugMode);
	engine.SetLightMode(myLightMode);
	engine.SetRenderMode(myRenderMode);
}

void ModelViewer::ActivateImGuiEditor()
{
	myImguiManager.Activate();
	myIsSceneActive = true;
}

void ModelViewer::ActivateSkeletonEditor()
{
	mySkeletonEditor.Activate();
	myIsSceneActive = false;
}

void ModelViewer::SetCameraSpeed(float aSpeed)
{
	myApplicationState.cameraSpeed = aSpeed;
	mySkeletonEditor.SetCameraSpeed(aSpeed);
}

void ModelViewer::SetMouseSensitivity(float aSensitivity)
{
	myApplicationState.cameraMouseSensitivity = aSensitivity;
	mySkeletonEditor.SetMouseSensitivity(aSensitivity);
}

void ModelViewer::ModelViewer::SaveState() const
{
	std::fstream fileStream(mySettingsPath, std::ios::out);
	if (fileStream)
	{
		Json::StreamWriterBuilder builder;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(myApplicationState, &fileStream);
		fileStream.flush();
	}
	else
	{
		myLogger.Err("Could not save settings!");
	}
	myLogger.Log("Successfully saved settings");
	fileStream.close();

	GraphicsEngine::Get().SaveSettings();
}

void ModelViewer::ModelViewer::LoadState()
{
	std::fstream fileStream(mySettingsPath, std::ios::in);
	if (fileStream)
	{
		Json::Value json;
		fileStream >> json;
		fileStream.flush();
		myApplicationState = json;
	}
	else
	{
		myLogger.Err("Could not load settings!");
	}
}

void ModelViewer::ShowSplashScreen()
{
	if (!mySplashWindow)
	{
		mySplashWindow = new SplashWindow();
		mySplashWindow->Init(myModuleHandle);
	}
}

void ModelViewer::HideSplashScreen() const
{
	mySplashWindow->Close();
	delete mySplashWindow;

	if (myApplicationState.startMaximized)
	{
		ShowWindow(myMainWindowHandle, SW_MAXIMIZE);
	}
	else
	{
		ShowWindow(myMainWindowHandle, SW_SHOW);
	}

	SetForegroundWindow(myMainWindowHandle);
}

void ModelViewer::SaveScene(const std::string& aPath, bool aAsBinary)
{
	Scene scene;
	scene.name = Crimson::GetFileNameWithoutExtension(aPath);
	scene.gameObjects.reserve(myObjectOrder.size());

	// TODO: Fix so an actual order can be saved
	for (auto& id : myObjectOrder)
	{
		scene.gameObjects.emplace(id, std::move(*myGameobjects.at(id)));
	}
	Engine::GetSceneManager().SaveScene(aPath, scene, aAsBinary);
	mySceneIsEdited = false;
	myLogger.Succ("Saved scene as: " + Crimson::MakeRelativeTo(aPath, "../"));
}

void ModelViewer::ModelViewer::LoadScene(const std::string& aPath)
{
	// TODO: Check if scene is edited and confirm loading a new file
	myUndoCommands.clear();
	myRedoCommands.clear();
	myObjectOrder.clear();
	myGameobjects.clear();

	if (aPath.empty())
	{
		Engine::GetSceneManager().UnloadActiveScene();
		mySceneName = "NewScene";
	}
	else
	{
		Engine::GetSceneManager().LoadScene(aPath);
		Scene& scene = Engine::GetSceneManager().GetActiveScene();
		mySceneName = scene.name;

		std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
		for (auto& [id, object] : scene.gameObjects)
		{
			GameObject copy = object;
			copy.CopyUuidOf(object);
			if (object.HasParent())
			{
				childlist.emplace(id, object.GetParent()->GetUUID());
			}
			myObjectOrder.emplace_back(id);
			myGameobjects.emplace(id, std::make_shared<GameObject>(std::move(copy)));
		}

		for (auto& [childID, parentID] : childlist)
		{
			myGameobjects.at(parentID)->AddChild(myGameobjects.at(childID).get());
		}
	}

	myImguiManager.Reset();
	SetPlayMode(false);

	for (auto& [id, object] : myGameobjects)
	{
		myImguiManager.AddGameObject(object.get());
	}

	myLogger.Succ("Loaded scene from: " + aPath);
}

void ModelViewer::Init()
{
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_SetAmbientlight>(nullptr, myApplicationState.ambientIntensity));
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetShadowBias>(myApplicationState.shadowBias));

	LoadScene("Default");
}

void ModelViewer::Update()
{
	GraphicsEngine& graphicsEngine = GraphicsEngine::Get();
	graphicsEngine.BeginFrame();
	Engine::BeginFrame();

	if (Engine::IsNetworkingEnabled())
	{
		HandleNetmessages();
	}

	myImguiManager.Update();
	mySkeletonEditor.Update();

	if (myIsSceneActive)
	{
		myCamera.Render();
		UpdateScene();
	}

	Engine::EndFrame();
	graphicsEngine.RenderFrame();

	//RHI::BeginEvent(L"NodeEditor Render");
	//myScriptGraphEditor->Render();
	//RHI::EndEvent();

	RHI::BeginEvent(L"ImGui Render");
	myImguiManager.Render();
	RHI::EndEvent();

	graphicsEngine.EndFrame();
}

void ModelViewer::HandleNetmessages()
{
	auto& messages = Engine::GetNetworkManager().GetMessages();
	for (auto& message : messages)
	{
		switch (message.type)
		{
		case Network::MessageType::Invalid:
		{
			break;
		}
		case Network::MessageType::Connect:
		{
			// This MessageType gets filtered before making it to this list, so this case should never be triggered
			break;
		}
		case Network::MessageType::Disconnect:
		{
			// Server has disconnected
			break;
		}
		case Network::MessageType::Confirmation:
		{
			break;
		}
		case Network::MessageType::Chat:
		{
			// This MessageType gets filtered before making it to this list, so this case should never be triggered
			break;
		}
		case Network::MessageType::Ping:
		{
			break;
		}
		case Network::MessageType::GameObjectMessage:
		{
			if (auto object = Engine::GetObjectManager().GetGameObject(Network::ExtractUUID(message)))
			{
				object->RecieveNetmessage(Network::ExtractGameObjectMessage(message));
			}
			break;
		}
		case Network::MessageType::CreateGameObject:
		{
			break;
		}
		case Network::MessageType::DeleteGameObject:
		{
			break;
		}
		default:
			break;
		}
	}
}

void ModelViewer::UpdateScene()
{
	if (myIsInPlayMode)
	{
		Engine::GetObjectManager().UpdateObjects();
	}
	else
	{
		for (auto& [id, object] : myGameobjects)
		{
			object->Render();
		}
	}
}

void ModelViewer::AddCommand(const std::shared_ptr<EditCommand>& aCommand)
{
	if (!myIsInPlayMode)
	{
		// Potential future problem with using infinite Undo-stack
		if (myRedoCommands.size() > 0)
		{
			myRedoCommands.clear();
		}
		aCommand->Execute();
		if (myUndoCommands.empty() || !myUndoCommands.back()->Merge(aCommand.get()))
		{
			myUndoCommands.emplace_back(aCommand);
		}
		mySceneIsEdited = true;
	}
	else
	{
		if (myPlayModeRedoCommands.size() > 0)
		{
			myPlayModeRedoCommands.clear();
		}
		aCommand->Execute();
		if (myPlayModeUndoCommands.empty() || !myPlayModeUndoCommands.back()->Merge(aCommand.get()))
		{
			myPlayModeUndoCommands.emplace_back(aCommand);
		}
	}

}

void ModelViewer::UndoCommand()
{
	if (!myIsInPlayMode)
	{
		if (myUndoCommands.size() > 0)
		{
			myUndoCommands.back()->Undo();
			myRedoCommands.emplace_back(myUndoCommands.back());
			myUndoCommands.pop_back();
			mySceneIsEdited = true;
		}
	}
	else
	{
		if (myPlayModeUndoCommands.size() > 0)
		{
			myPlayModeUndoCommands.back()->Undo();
			myPlayModeRedoCommands.emplace_back(myPlayModeUndoCommands.back());
			myPlayModeUndoCommands.pop_back();
			mySceneIsEdited = true;
		}
	}
}

void ModelViewer::RedoCommand()
{
	if (!myIsInPlayMode)
	{
		if (myRedoCommands.size() > 0)
		{
			myRedoCommands.back()->Execute();
			myUndoCommands.emplace_back(myRedoCommands.back());
			myRedoCommands.pop_back();
			mySceneIsEdited = true;
		}
	}
	else
	{
		if (myPlayModeRedoCommands.size() > 0)
		{
			myPlayModeRedoCommands.back()->Execute();
			myPlayModeUndoCommands.emplace_back(myPlayModeRedoCommands.back());
			myPlayModeRedoCommands.pop_back();
			mySceneIsEdited = true;
		}
	}
}

void ModelViewer::ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	if (!myIsSceneActive)
	{
		return;
	}
	auto& engine = GraphicsEngine::Get();
	if (anEvent == Crimson::eInputEvent::KeyDown)
	{
		switch (aKey)
		{
		case Crimson::eKey::F1:
		{
			SetPlayMode(!myIsInPlayMode);
			break;
		}
		case Crimson::eKey::F4:
		{
			engine.NextToneMap();
			break;
		}
		case Crimson::eKey::F5:
		{
			myDebugMode = engine.NextDebugMode();
			break;
		}
		case Crimson::eKey::F6:
		{
			myLightMode = engine.NextLightMode();
			break;
		}
		case Crimson::eKey::F7:
		{
			myRenderMode = engine.NextRenderMode();
			break;
		}
		case Crimson::eKey::F8:
		{
			myDebugMode = engine.SetDebugMode(GraphicsEngine::DebugMode::Default);
			myLightMode = engine.SetLightMode(GraphicsEngine::LightMode::Default);
			myRenderMode = engine.SetRenderMode(GraphicsEngine::RenderMode::Mesh);
			break;
		}
		default:
			break;
		}
	}
}

void ModelViewer::ReceiveEvent(Crimson::eInputAction anAction, float aValue)
{
	if (!myIsSceneActive)
	{
		return;
	}
	if (aValue < 1.5f)
	{
		return;
	}

	if (anAction == Crimson::eInputAction::Undo)
	{
		UndoCommand();
	}
	else if (anAction == Crimson::eInputAction::Redo)
	{
		RedoCommand();
	}
}