#include "Modelviewer.h"
#include "Windows/SplashWindow.h"
#include "Commands/EditCmd_AddGameobject.h"
#include "Commands/EditCmd_RemoveGameobject.h"

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/InterOp/Helpers.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/DirectoryFunctions.h"

#include <Timer.h>
#include <InputMapper.h>

#include <filesystem>

ModelViewer::ModelViewer() : myModuleHandle(nullptr), myMainWindowHandle(nullptr), mySplashWindow(nullptr), mySettingsPath("Settings/mw_settings.json"), myApplicationState(), myLogger(), myCamera(), myGameObjects()
#ifndef _RETAIL
, myDebugMode(GraphicsEngine::DebugMode::Default), myLightMode(GraphicsEngine::LightMode::Default), myRenderMode(GraphicsEngine::RenderMode::Mesh), myImguiManager()
#endif // _RETAIL
{
}

bool ModelViewer::Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess)
{
	myLogger = Logger::Create("ModelViewer");
	myModuleHandle = aHInstance;
	LoadState();

	constexpr LPCWSTR windowClassName = L"ModelViewerMainWindow";

	// First we create our Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = aWindowProcess;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = windowClassName;
	RegisterClass(&windowClass);

	std::wstring stdTitle{ Helpers::string_cast<std::wstring>(myApplicationState.WindowTitle) };
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
	auto& input = *CommonUtilities::InputMapper::GetInstance();
	input.Init(myMainWindowHandle);
	AssetManager::Init();
#ifdef _RETAIL
	GraphicsEngine::Get().Initialize(myMainWindowHandle, false);
#else
	GraphicsEngine::Get().Initialize(myMainWindowHandle, true);
#endif // _RETAIL	
	//myCamera.Init({ static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy) });
	myCamera.Init(myApplicationState.WindowSize, myApplicationState.CameraSpeed, myApplicationState.CameraRotationSpeed, myApplicationState.CameraMouseSensitivity);
	AssetManager::GeneratePrimitives();

#ifndef _RETAIL
	AssetManager::PreLoadAssets();
	myImguiManager.Init();

	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F5);
	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F6);
	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F7);
	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F8);

	input.BindAction(CommonUtilities::eInputAction::Undo, CommonUtilities::KeyBind{ CommonUtilities::eKey::Z, CommonUtilities::eKey::Ctrl });
	input.BindAction(CommonUtilities::eInputAction::Redo, CommonUtilities::KeyBind{ CommonUtilities::eKey::Y, CommonUtilities::eKey::Ctrl });
	input.Attach(this, CommonUtilities::eInputAction::Undo);
	input.Attach(this, CommonUtilities::eInputAction::Redo);

	DragAcceptFiles(myMainWindowHandle, TRUE);
#endif // _RETAIL

	HideSplashScreen();

	return true;
}

int ModelViewer::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//LoadScene("Default");
	Init();
	CommonUtilities::Timer::Init();

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
		Update();
	}

#ifndef _RETAIL
	myImguiManager.Release();
#endif // _RETAIL

	return 0;
}

#ifndef _RETAIL
void ModelViewer::SetDropFile(HDROP aHandle)
{
	myImguiManager.SetDropFile(aHandle);
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

	return myGameObjects.at(newObject->GetID());
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
	
	return myGameObjects.at(anObject->GetID());
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
	return myGameObjects.at(id);
}

std::shared_ptr<GameObject> ModelViewer::GetGameObject(unsigned anID)
{
	assert(anID != 0 && "Incorrect ID!");

	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
		return iter->second;
	}
	return nullptr;
}

bool ModelViewer::RemoveGameObject(unsigned anID)
{
	assert(anID != 0 && "Incorrect ID!");

	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
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
	auto iter = myGameObjects.emplace(newobject->GetID(), std::move(newObject));
	return iter.first->second;
}

GameObject& ModelViewer::AddGameObject(const GameObject& anObject)
{
	GameObject newObject(anObject);
	return myGameObjects.emplace(newobject->GetID(), std::move(newObject)).first->second;
}

GameObject& ModelViewer::AddGameObject(GameObject&& anObject)
{
	return myGameObjects.emplace(anobject->GetID(), std::move(anObject)).first->second;
}

GameObject* ModelViewer::GetGameObject(unsigned anID)
{
	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
		return &iter->second;
	}
	return nullptr;
}

bool ModelViewer::RemoveGameObject(unsigned anID)
{
	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
		myGameObjects.erase(iter);
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
	std::string path = AddExtensionIfMissing(aPath, GetSceneExtension());
	path = CreateValidPath(path, GetScenePath(), &myLogger);
	if (path.empty())
	{
		myLogger.Err("Could not create filepath: " + aPath);
	}
	Json::Value scene;
	scene["GameObjectIDCount"] = GameObject::GetIDCount();
	scene["GameObjects"] = Json::arrayValue;

	int i = 0;
	for (auto& [id, object] : myGameObjects)
	{
#ifndef _RETAIL
		scene["GameObjects"][i] = object->ToJson();
		scene["GameObjects"][i].setComment("// GameObject ID: " + std::to_string(object->GetID()), Json::commentBefore);
#else
		scene["GameObjects"][i] = object->ToJson();
		scene["GameObjects"][i].setComment("// GameObject ID: " + std::to_string(object->GetID()), Json::commentBefore);
#endif // !_RETAIL

		i++;
	}

	std::fstream fileStream(path, std::ios::out);
	if (fileStream)
	{
		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "All";
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(scene, &fileStream);
		fileStream.flush();
	}
	else
	{
		myLogger.Err("Could not open file at: " + aPath);
	}
	myLogger.Log("Successfully saved scene to: " + path);
	fileStream.close();
}

void ModelViewer::ModelViewer::LoadScene(const std::string& aPath)
{
	std::string path = AddExtensionIfMissing(aPath, GetSceneExtension());
	path = GetValidPath(path, GetScenePath(), &myLogger);
	if (path.empty())
	{
		myLogger.Err("Could not find filepath: " + aPath);
	}

	Json::Value json;
	std::fstream fileStream(path, std::ios::in);
	if (fileStream)
	{
		fileStream >> json;
	}
	else
	{
		myLogger.Err("Could not open file at: " + aPath);
	}
	fileStream.close();

	GameObject::SetIDCount(json["GameObjectIDCount"].asUInt());
	for (auto& object : json["GameObjects"])
	{
		AddGameObject(object, false);
	}
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
	CommonUtilities::Timer::Update();
	CommonUtilities::InputMapper::GetInstance()->Notify();

#ifndef _RETAIL
	myImguiManager.Update();
#endif // _RETAIL

	myCamera.Update();
	UpdateScene();

	CommonUtilities::InputMapper::GetInstance()->Update();
	engine.RenderFrame();

#ifndef _RETAIL
	myImguiManager.Render();
#endif // _RETAIL

	engine.EndFrame();
}

void ModelViewer::UpdateScene()
{
	for (auto& [id, object] : myGameObjects)
	{
#ifndef _RETAIL
		object->Update();
#else
		object->Update();
#endif // !_RETAIL
	}
}

#ifndef _RETAIL
void ModelViewer::AddCommand(const std::shared_ptr<EditCommand>& aCommand)
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

void ModelViewer::ReceiveEvent(CommonUtilities::eInputEvent, CommonUtilities::eKey aKey)
{
	auto& engine = GraphicsEngine::Get();
	switch (aKey)
	{
	case CommonUtilities::eKey::F5:
	{
		myDebugMode = engine.NextDebugMode();
		break;
	}
	case CommonUtilities::eKey::F6:
	{
		myLightMode = engine.NextLightMode();
		break;
	}
	case CommonUtilities::eKey::F7:
	{
		myRenderMode = engine.NextRenderMode();
		break;
	}
	case CommonUtilities::eKey::F8:
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

void ModelViewer::ReceiveEvent(CommonUtilities::eInputAction anAction, float aValue)
{
	if (aValue < 1.5f)
	{
		return;
	}

	if (anAction == CommonUtilities::eInputAction::Undo)
	{
		UndoCommand();
	}
	else if(anAction == CommonUtilities::eInputAction::Redo)
	{
		RedoCommand();
	}
}
#endif // _RETAIL
