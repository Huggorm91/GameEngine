#include "Modelviewer.pch.h"
#include "Modelviewer.h"
#include "Windows/SplashWindow.h"
#include "Commands/EditCmd_AddGameobject.h"
#include "Commands/EditCmd_RemoveGameobject.h"

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/Assets/Binary.h"
#include "File/DirectoryFunctions.h"

#include "Time/Timer.h"
#include "Input/InputMapper.h"
#include "Json\jsonCpp\json.h"


ModelViewer::ModelViewer() : myModuleHandle(nullptr), myMainWindowHandle(nullptr), mySplashWindow(nullptr), mySettingsPath("Settings/mw_settings.json"), myApplicationState(), myLogger(), myCamera(), myScene()
#ifndef _RETAIL
, myDebugMode(GraphicsEngine::DebugMode::Default), myLightMode(GraphicsEngine::LightMode::Default), myRenderMode(GraphicsEngine::RenderMode::Mesh), myImguiManager(), myIsInPlayMode(false), myIsMaximized(false), myPlayScene(), myPlayScenePointers()
#endif // _RETAIL
{
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
	std::string saveName = "Bin\\Crashdump\\" + Crimson::FileNameTimestamp() + "_" + myLoadedScene;
	try
	{
		SaveScene("..\\" + saveName);
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

	constexpr LPCWSTR windowClassName = L"ModelViewerMainWindow";

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
	auto& input = *Crimson::InputMapper::GetInstance();
	input.Init(myMainWindowHandle);
#ifdef _RETAIL
	GraphicsEngine::Get().Initialize(myMainWindowHandle, false);
#else
	GraphicsEngine::Get().Initialize(myMainWindowHandle, true);
#endif // _RETAIL
	myCamera.Init(myApplicationState.WindowSize, myApplicationState.CameraSpeed, myApplicationState.CameraRotationSpeed, myApplicationState.CameraMouseSensitivity);
	AssetManager::Init();
	AssetManager::GeneratePrimitives();

#ifndef _RETAIL
	AssetManager::PreLoadAssets();
	myImguiManager.Init();
	myImguiManager.SetActiveObjects(&myScene.GameObjects);

	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F4);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F5);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F6);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F7);
	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::F8);

	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::Pause);

	input.BindAction(Crimson::eInputAction::Undo, Crimson::KeyBind{ Crimson::eKey::Z, Crimson::eKey::Ctrl });
	input.BindAction(Crimson::eInputAction::Redo, Crimson::KeyBind{ Crimson::eKey::Y, Crimson::eKey::Ctrl });
	input.Attach(this, Crimson::eInputAction::Undo);
	input.Attach(this, Crimson::eInputAction::Redo);

	DragAcceptFiles(myMainWindowHandle, TRUE);
#endif // _RETAIL

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
		try
		{
			Update();
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
	unsigned id = anObject.GetID();
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

std::shared_ptr<GameObject> ModelViewer::GetGameObject(unsigned anID)
{
	assert(anID != 0 && "Incorrect ID!");

	if (auto iter = myScene.GameObjects.find(anID); iter != myScene.GameObjects.end())
	{
		return iter->second;
	}
	return nullptr;
}

bool ModelViewer::RemoveGameObject(unsigned anID)
{
	assert(anID != 0 && "Incorrect ID!");

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

GameObject* ModelViewer::GetGameObject(unsigned anID)
{
	if (auto iter = myScene.GameObjects.find(anID); iter != myScene.GameObjects.end())
	{
		return &iter->second;
	}
	return nullptr;
}

bool ModelViewer::RemoveGameObject(unsigned anID)
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

void ModelViewer::ModelViewer::SaveScene(const std::string& aPath) const
{
	AssetManager::SaveAsset(myScene, aPath, false);
}

void ModelViewer::ModelViewer::LoadScene(const std::string& aPath)
{
	myLoadedScene = aPath;
#ifndef _RETAIL
	myScene = AssetManager::GetAsset<EditorScene>(aPath);
	for (auto& [id, object] : myScene.GameObjects)
	{
		myImguiManager.AddGameObject(object.get());
	}
#else
	myScene = AssetManager::GetAsset<Scene>(aPath);
#endif // !_RETAIL
}

void ModelViewer::Init()
{
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_SetAmbientlight>(nullptr, myApplicationState.AmbientIntensity));
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetShadowBias>(myApplicationState.ShadowBias));
	LoadScene("Default");
}

void ModelViewer::Update()
{
	GraphicsEngine& engine = GraphicsEngine::Get();
	engine.BeginFrame();
	Crimson::Timer::Update();
	Crimson::InputMapper::GetInstance()->Notify();

#ifndef _RETAIL
	myImguiManager.Update();
#endif // _RETAIL

	myCamera.Update();
	UpdateScene();

	Crimson::InputMapper::GetInstance()->Update();
	engine.RenderFrame();

#ifndef _RETAIL
	myImguiManager.Render();
#endif // _RETAIL

	engine.EndFrame();
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
	if (myIsInPlayMode)
	{
		return;
	}

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
}

void ModelViewer::UndoCommand()
{
	if (myUndoCommands.size() > 0)
	{
		myUndoCommands.back()->Undo();
		myRedoCommands.emplace_back(myUndoCommands.back());
		myUndoCommands.pop_back();
	}
}

void ModelViewer::RedoCommand()
{
	if (myRedoCommands.size() > 0)
	{
		myRedoCommands.back()->Execute();
		myUndoCommands.emplace_back(myRedoCommands.back());
		myRedoCommands.pop_back();
	}
}

void ModelViewer::ReceiveEvent(Crimson::eInputEvent, Crimson::eKey aKey)
{
	if (aKey == Crimson::eKey::Pause)
	{
		myIsInPlayMode = !myIsInPlayMode;
		if (myIsInPlayMode)
		{
			myPlayScene.GameObjects.reserve(myScene.GameObjects.size());
			for (auto& [id, object] : myScene.GameObjects)
			{
				auto copy = *object;
				copy.CopyIDsOf(*object, true);
				myPlayScene.GameObjects.emplace(id, copy);
				myPlayScenePointers.emplace(id, std::shared_ptr<GameObject>(&myPlayScene.GameObjects.at(id), [](GameObject*){}));
			}
			myImguiManager.SetActiveObjects(&myPlayScenePointers);
		}
		else
		{
			myImguiManager.SetActiveObjects(&myScene.GameObjects);
			myPlayScenePointers.clear();
			myPlayScene = Scene();
		}
	}

	auto& engine = GraphicsEngine::Get();
	switch (aKey)
	{
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

void ModelViewer::ReceiveEvent(Crimson::eInputAction anAction, float aValue)
{
	if (myIsInPlayMode)
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
#endif // _RETAIL
