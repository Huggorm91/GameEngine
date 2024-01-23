#include "Modelviewer.pch.h"
#include "Modelviewer.h"
#include "Windows/SplashWindow.h"
#include "Commands/EditCmd_AddGameobject.h"
#include "Commands/EditCmd_RemoveGameobject.h"

#include "Engine/Engine.h"

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/Assets/Binary.h"
#include "AssetManager/Managers/CollisionManager.h"
#include "AssetManager/Assets/Components/Camera/PerspectiveCameraComponent.h"

#include "File/DirectoryFunctions.h"
#include "Time/Timer.h"
#include "Input/InputMapper.h"
#include "PostMaster/PostMaster.h"
#include "Json\jsonCpp\json.h"


ModelViewer::ModelViewer() : myIsAcceptingInput(false), myModuleHandle(nullptr), myMainWindowHandle(nullptr), mySplashWindow(nullptr), mySettingsPath("Settings/mw_settings.json"), myApplicationState(), myLogger(), myCamera(), myScene()
#ifndef _RETAIL
, myDebugMode(GraphicsEngine::DebugMode::Default), myLightMode(GraphicsEngine::LightMode::Default), myRenderMode(GraphicsEngine::RenderMode::Mesh), myImguiManager()
, myIsInPlayMode(false), myIsMaximized(false), myPlayScene(), myPlayScenePointers(), mySceneIsEdited(false)
#endif // _RETAIL
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

	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::Q);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::E);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::W);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::A);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::S);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::D);

	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::MouseRightButton);
	input.Attach(this, Crimson::eInputEvent::KeyUp, Crimson::eKey::MouseRightButton);

	input.BindAction(Crimson::eInputAction::Undo, Crimson::KeyBind{ Crimson::eKey::Z, Crimson::eKey::Ctrl });
	input.BindAction(Crimson::eInputAction::Redo, Crimson::KeyBind{ Crimson::eKey::Y, Crimson::eKey::Ctrl });
	input.BindAction(Crimson::eInputAction::Duplicate, Crimson::KeyBind{ Crimson::eKey::D, Crimson::eKey::Ctrl });

	input.Attach(this, Crimson::eInputAction::Undo);
	input.Attach(this, Crimson::eInputAction::Redo);
	input.Attach(this, Crimson::eInputAction::Duplicate);
}

void ModelViewer::HandleCrash(const std::exception& anException)
{
	// Center console and bring it to the front
	{
		HWND consoleWindow = GetConsoleWindow();
		RECT consolePos;
		GetWindowRect(consoleWindow, &consolePos);
		consolePos.right = consolePos.right - consolePos.left;
		consolePos.bottom = consolePos.bottom - consolePos.top;

		RECT windowRect;
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
	myLogger.SetPrintToFile(true, "Logs\\" + Crimson::FileNameTimestamp() + "_Log.txt");
	myLogger.LogException(anException);

	// Save current scene if possible
	std::string saveName = "Bin\\Crashdump\\" + Crimson::FileNameTimestamp() + "_" + myScene.Name;
	try
	{
		SaveScene("..\\" + saveName, false);
		myLogger.Succ("Saved current scene to: " + saveName);
	}
	catch (...)
	{
		myLogger.Err("Failed to save current scene to: " + saveName);
	}

	// Leave console up to let user read information
	system("PAUSE");
}

bool ModelViewer::Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess)
{
	myLogger = Logger::Create("ModelViewer");
	myModuleHandle = aHInstance;
	LoadState();

	myLogger.Log(Crimson::GetAppPath());

	constexpr LPCWSTR windowClassName = L"CrimsonEngine_MainWindow";

	// First we create our Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = aWindowProcess;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = windowClassName;
	RegisterClass(&windowClass);

	std::wstring stdTitle{ Crimson::ToWString(myApplicationState.WindowTitle) };
	LPCWSTR title{ stdTitle.c_str() };

	DWORD flags;
	if (myApplicationState.StartMaximized)
	{
		flags = WS_OVERLAPPEDWINDOW | WS_POPUP | WS_MAXIMIZE;
	}
	else
	{
		flags = WS_OVERLAPPEDWINDOW | WS_POPUP;
	}
	myIsMaximized = myApplicationState.StartMaximized;

	// Get center of screen
	RECT windowRect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &windowRect, 0);
	//GetClientRect(GetDesktopWindow(), &windowRect);

	windowRect.left = static_cast<LONG>((windowRect.right * 0.5f) - (myApplicationState.WindowSize.x * 0.5f));
	windowRect.top = static_cast<LONG>((windowRect.bottom * 0.5f) - (myApplicationState.WindowSize.y * 0.5f));
	windowRect.right = myApplicationState.WindowSize.x;
	windowRect.bottom = myApplicationState.WindowSize.y;

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

	// TODO: Load all settings from json
	Engine::Init(myMainWindowHandle, myApplicationState.WindowSize);
#ifdef _RETAIL
	GraphicsEngine::Get().Initialize(myMainWindowHandle, false);
#else
	GraphicsEngine::Get().Initialize(myMainWindowHandle, true);
#endif // _RETAIL

	AssetManager::Init();
	AssetManager::GeneratePrimitives();

#ifndef _RETAIL
	AssetManager::PreLoadAssets();
	myImguiManager.Init();

	myScriptGraphEditor.Init();

	SetKeyBinds();

	DragAcceptFiles(myMainWindowHandle, TRUE);
#endif // _RETAIL

	myCamera.AddComponent(PerspectiveCameraComponent(90.f, 1.f, 10000.f));
	myCamera.MarkAsPrefab();
	myCamera.SetPosition({ 0.f, 200.f, 0.f });

	HideSplashScreen();

	return true;
}

int ModelViewer::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Init();
	Crimson::Timer::Init();

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
		// REMEMBER!
		// The frame update for the game does NOT happen inside the PeekMessage loop.
		// This would cause the game to only update if there are messages and also run
		// the update several times per frame (once for each message).
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
			HandleCrash(anException);
		}
		catch (...)
		{
			isRunning = false;
			HandleCrash(std::invalid_argument("Caught unknown Error!"));
		}
#endif // _DEBUG
	}

#ifndef _RETAIL
	myImguiManager.Release();
#endif // _RETAIL

	return 0;
}

#ifndef _RETAIL
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
	if (myIsInPlayMode)
	{
		myPlayScene.GameObjects.reserve(myScene.GameObjects.size());
		std::unordered_map<GameObjectID, GameObjectID> childlist;
		for (auto& [id, object] : myScene.GameObjects)
		{
			if (object->HasParent())
			{
				childlist.emplace(id, object->GetParent()->GetID());
			}
			auto copy = *object;
			copy.CopyIDsOf(*object, true);
			myPlayScene.GameObjects.emplace(id, std::move(copy));
			myPlayScenePointers.emplace(id, std::shared_ptr<GameObject>(&myPlayScene.GameObjects.at(id), [](GameObject*){}));
		}

		for (auto& [childID, parentID] : childlist)
		{
			myPlayScene.GameObjects.at(parentID).AddChild(&myPlayScene.GameObjects.at(childID));
		}

		myImguiManager.SetActiveObjects(&myPlayScenePointers);
	}
	else
	{
		myImguiManager.SetActiveObjects(&myScene.GameObjects);
		myPlayScenePointers.clear();
		myPlayScene = Scene();
		myPlayModeRedoCommands.clear();
		myPlayModeUndoCommands.clear();
	}
}

std::shared_ptr<GameObject>& ModelViewer::AddGameObject(bool aAddToUndo)
{
	std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>();
	if (aAddToUndo)
	{
		AddCommand(std::make_shared<EditCmd_AddGameObject>(newObject));
	}
	else
	{
		EditCmd_AddGameObject command(newObject);
		command.Execute();
	}

	return myScene.GameObjects.at(newObject->GetID());
}

std::shared_ptr<GameObject>& ModelViewer::AddGameObject(const std::shared_ptr<GameObject>& anObject, bool aAddToUndo)
{
	if (aAddToUndo)
	{
		AddCommand(std::make_shared<EditCmd_AddGameObject>(anObject));
	}
	else
	{
		EditCmd_AddGameObject command(anObject);
		command.Execute();
	}

	return myScene.GameObjects.at(anObject->GetID());
}

std::shared_ptr<GameObject>& ModelViewer::AddGameObject(GameObject&& anObject, bool aAddToUndo)
{
	GameObjectID id = anObject.GetID();
	if (aAddToUndo)
	{
		AddCommand(std::make_shared<EditCmd_AddGameObject>(std::move(anObject)));
	}
	else
	{
		EditCmd_AddGameObject command(std::move(anObject));
		command.Execute();
	}
	return myScene.GameObjects.at(id);
}

std::shared_ptr<GameObject> ModelViewer::GetGameObject(GameObjectID anID)
{
	assert(anID != GameObjectID::Invalid && "Incorrect ID!");

	if (auto iter = myScene.GameObjects.find(anID); iter != myScene.GameObjects.end())
	{
		return iter->second;
	}
	return nullptr;
}

bool ModelViewer::RemoveGameObject(GameObjectID anID)
{
	assert(anID != GameObjectID::Invalid && "Incorrect ID!");

	if (auto iter = myScene.GameObjects.find(anID); iter != myScene.GameObjects.end())
	{
		AddCommand(std::make_shared<EditCmd_RemoveGameObject>(iter->second));
		return true;
	}
	return false;
}
#else
GameObject& ModelViewer::AddGameObject()
{
	GameObject newObject;
	auto iter = myScene.GameObjects.emplace(newobject->GetID(), std::move(newObject));
	return iter.first->second;
}

GameObject& ModelViewer::AddGameObject(const GameObject& anObject)
{
	GameObject newObject(anObject);
	return myScene.GameObjects.emplace(newobject->GetID(), std::move(newObject)).first->second;
}

GameObject& ModelViewer::AddGameObject(GameObject&& anObject)
{
	return myScene.GameObjects.emplace(anobject->GetID(), std::move(anObject)).first->second;
}

GameObject* ModelViewer::GetGameObject(GameObjectID anID)
{
	if (auto iter = myScene.GameObjects.find(anID); iter != myScene.GameObjects.end())
	{
		return &iter->second;
	}
	return nullptr;
}

bool ModelViewer::RemoveGameObject(GameObjectID anID)
{
	if (auto iter = myScene.GameObjects.find(anID); iter != myScene.GameObjects.end())
	{
		myScene.GameObjects.erase(iter);
		return true;
	}
	return false;
}
#endif // _RETAIL

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
	fileStream.close();
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

	if (myApplicationState.StartMaximized)
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
	myScene.Name = Crimson::GetFileNameWithoutExtension(aPath);
	myScene.Path = aPath;
	myScene.GameObjectIDCount = GameObject::GetIDCount();
	AssetManager::SaveAsset(myScene, aPath, aAsBinary);
	mySceneIsEdited = false;
	myLogger.Succ("Saved scene as: " + Crimson::MakeRelativeTo(aPath, "../"));
}

void ModelViewer::ModelViewer::LoadScene(const std::string& aPath)
{
	// TODO: Check if scene is edited and confirm loading a new file
	myUndoCommands.clear();
	myRedoCommands.clear();

	if (aPath.empty())
	{
		myScene = {};
	}

#ifndef _RETAIL
	else
	{
		myScene = AssetManager::GetAsset<EditorScene>(aPath);
	}
	myImguiManager.Reset();
	SetPlayMode(false);

	for (auto& [id, object] : myScene.GameObjects)
	{
		myImguiManager.AddGameObject(object.get());
	}
#else
	else
	{
		myScene = AssetManager::GetAsset<Scene>(aPath);
	}
#endif // !_RETAIL
	SetGameObjectIDCount(myScene.GameObjectIDCount);
	myLogger.Succ("Loaded scene from: " + Crimson::MakeRelativeTo(myScene.Path, "../"));
}

void ModelViewer::Init()
{
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_SetAmbientlight>(nullptr, myApplicationState.AmbientIntensity));
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetShadowBias>(myApplicationState.ShadowBias));
	LoadScene("Default");
}

void ModelViewer::Update()
{
	GraphicsEngine::Get().BeginFrame();
	Engine::BeginFrame();

#ifndef _RETAIL
	myImguiManager.Update();
	myScriptGraphEditor.Update(Crimson::Timer::GetDeltaTime());
#endif // _RETAIL

	myCamera.Update();
	UpdateScene();

	Engine::EndFrame();
	GraphicsEngine::Get().RenderFrame();

#ifndef _RETAIL
	myScriptGraphEditor.Render();
	myImguiManager.Render();
#endif // _RETAIL

	GraphicsEngine::Get().EndFrame();
}

void ModelViewer::UpdateScene()
{
#ifndef _RETAIL
	if (myIsInPlayMode)
	{
		for (auto& [id, object] : myPlayScene.GameObjects)
		{
			object.Update();
		}
	}
	else
	{
		for (auto& [id, object] : myScene.GameObjects)
		{
			object->Render();
		}
	}
#else
	for (auto& [id, object] : myScene.GameObjects)
	{
		object.Update();
	}
#endif // !_RETAIL

}

#ifndef _RETAIL
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
	switch (anEvent)
	{
	case Crimson::eInputEvent::MouseMove:
	{
		const float multiplier = myApplicationState.CameraMouseSensitivity * .5f;
		const Crimson::Vector2f distance = Engine::GetInputMapper().GetMouseMovement();
		Crimson::Vector3f rotation = myCamera.GetComponent<PerspectiveCameraComponent>().GetRotationDegree();
		rotation.x += distance.y * multiplier;
		rotation.y += distance.x * multiplier;
		myCamera.GetComponent<PerspectiveCameraComponent>().SetRotation(rotation);
		break;
	}
	case Crimson::eInputEvent::KeyDown:
	{
		switch (aKey)
		{
		case Crimson::eKey::MouseRightButton:
		{
			auto& inputHandler = Engine::GetInputMapper();
			inputHandler.Attach(this, Crimson::eInputEvent::MouseMove);
			inputHandler.CaptureMouse(true);
			inputHandler.HideMouse();
			myIsAcceptingInput = true;
			break;
		}
		case Crimson::eKey::F1:
		{
			SetPlayMode(!myIsInPlayMode);
			break;
		}
		case Crimson::eKey::F4:
		{
			GraphicsEngine::Get().NextToneMap();
			break;
		}
		case Crimson::eKey::F5:
		{
			myDebugMode = GraphicsEngine::Get().NextDebugMode();
			break;
		}
		case Crimson::eKey::F6:
		{
			myLightMode = GraphicsEngine::Get().NextLightMode();
			break;
		}
		case Crimson::eKey::F7:
		{
			myRenderMode = GraphicsEngine::Get().NextRenderMode();
			break;
		}
		case Crimson::eKey::F8:
		{
			myDebugMode = GraphicsEngine::Get().SetDebugMode(GraphicsEngine::DebugMode::Default);
			myLightMode = GraphicsEngine::Get().SetLightMode(GraphicsEngine::LightMode::Default);
			myRenderMode = GraphicsEngine::Get().SetRenderMode(GraphicsEngine::RenderMode::Mesh);
			break;
		}
		default:
			break;
		}
		break;
	}
	case Crimson::eInputEvent::KeyUp:
	{
		if (aKey == Crimson::eKey::MouseRightButton)
		{
			auto& inputHandler = Engine::GetInputMapper();
			inputHandler.Detach(this, Crimson::eInputEvent::MouseMove);
			inputHandler.ReleaseMouse();
			inputHandler.ShowMouse();
			myIsAcceptingInput = false;
		}
		break;
	}
	case Crimson::eInputEvent::KeyHeld:
	{
		if (myIsAcceptingInput == false)
		{
			return;
		}

		float multiplier = Engine::GetInputMapper().GetKeyHeld(Crimson::eKey::Shift) ? 2.f : Engine::GetInputMapper().GetKeyHeld(Crimson::eKey::Shift) ? 2.f : 1.f;
		float timeDelta = Crimson::Timer::GetDeltaTime();
		const auto& rotation = myCamera.GetComponent<PerspectiveCameraComponent>().GetRotationRadian();

		switch (aKey)
		{
		case Crimson::eKey::Q:
		{
			Crimson::Vector4f movement = {};
			movement.y = myApplicationState.CameraSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(rotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(rotation.y);
			myCamera.SetPosition(myCamera.GetWorldPosition() - movement);
			break;
		}
		case Crimson::eKey::E:
		{
			Crimson::Vector4f movement = {};
			movement.y = myApplicationState.CameraSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(rotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(rotation.y);
			myCamera.SetPosition(myCamera.GetWorldPosition() + movement);
			break;
		}
		case Crimson::eKey::W:
		{
			Crimson::Vector4f movement = {};
			movement.z = myApplicationState.CameraSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(rotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(rotation.y);
			myCamera.SetPosition(myCamera.GetWorldPosition() + movement);
			break;
		}
		case Crimson::eKey::S:
		{
			Crimson::Vector4f movement = {};
			movement.z = myApplicationState.CameraSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(rotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(rotation.y);
			myCamera.SetPosition(myCamera.GetWorldPosition() - movement);
			break;
		}
		case Crimson::eKey::A:
		{
			Crimson::Vector4f movement = {};
			movement.x = myApplicationState.CameraSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(rotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(rotation.y);
			myCamera.SetPosition(myCamera.GetWorldPosition() - movement);
			break;
		}
		case Crimson::eKey::D:
		{
			Crimson::Vector4f movement = {};
			movement.x = myApplicationState.CameraSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(rotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(rotation.y);
			myCamera.SetPosition(myCamera.GetWorldPosition() + movement);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void ModelViewer::ReceiveEvent(Crimson::eInputAction anAction, float aValue)
{
	if (aValue < 1.5f)
	{
		return;
	}

	switch (anAction)
	{
	case Crimson::eInputAction::Undo:
	{
		UndoCommand();
		break;
	}
	case Crimson::eInputAction::Redo:
	{
		RedoCommand();
		break;
	}
	case Crimson::eInputAction::Duplicate:
	{
		myScriptGraphEditor.DuplicateSelection();
		break;
	}
	default:
		break;
	}
}
#endif // _RETAIL

void ModelViewer::RecieveMessage(const Crimson::Message& aMessage)
{
	if (const Crimson::Vector2i* ids = aMessage.GetDataAsVector2i(); ids != nullptr)
	{
		ScriptGraphNodePayload payload;
		switch (aMessage.GetMessageType())
		{
		case Crimson::eMessageType::Collision_OnTriggerEnter:
			payload.SetVariable("Trigger ID", ids->x);
			payload.SetVariable("Collider ID", ids->y);
			myScriptGraphEditor.TriggerEntryPoint("On Trigger Enter", payload);
			break;
		case Crimson::eMessageType::Collision_OnTriggerExit:
			payload.SetVariable("Trigger ID", ids->x);
			payload.SetVariable("Collider ID", ids->y);
			myScriptGraphEditor.TriggerEntryPoint("On Trigger Exit", payload);
			break;
		case Crimson::eMessageType::Collision_OnTriggerStay:
			payload.SetVariable("Trigger ID", ids->x);
			payload.SetVariable("Collider ID", ids->y);
			myScriptGraphEditor.TriggerEntryPoint("On Trigger Stay", payload);
			break;
		case Crimson::eMessageType::Collision_OnCollisionEnter:
			payload.SetVariable("Collider1 ID", ids->x);
			payload.SetVariable("Collider2 ID", ids->y);
			myScriptGraphEditor.TriggerEntryPoint("On Collision Enter", payload);
			break;
		case Crimson::eMessageType::Collision_OnCollisionExit:
			payload.SetVariable("Collider1 ID", ids->x);
			payload.SetVariable("Collider2 ID", ids->y);
			myScriptGraphEditor.TriggerEntryPoint("On Collision Exit", payload);
			break;
		case Crimson::eMessageType::Collision_OnCollisionStay:
			payload.SetVariable("Collider1 ID", ids->x);
			payload.SetVariable("Collider2 ID", ids->y);
			myScriptGraphEditor.TriggerEntryPoint("On Collision Stay", payload);
			break;
		default:
			break;
		}
	}
	else if (const int* id = aMessage.GetDataAsInt(); id != nullptr)
	{
		ScriptGraphNodePayload payload;
		payload.SetVariable("Object ID", id);

		switch (aMessage.GetMessageType())
		{
		case Crimson::eMessageType::GameObject_TakeDamage:
			myScriptGraphEditor.TriggerEntryPoint("Damage Taken", payload);
			break;
		case Crimson::eMessageType::GameObject_Died:
			myScriptGraphEditor.TriggerEntryPoint("Object Died", payload);
			break;
		default:
			break;
		}
	}
}
