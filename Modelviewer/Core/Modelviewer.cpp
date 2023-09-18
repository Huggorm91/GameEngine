
// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_stdlib.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_win32.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_dx11.h"

#include "Modelviewer.h"
#include "Windows/SplashWindow.h"
#include "Commands/EditCmd_AddGameObject.h"
#include "Commands/EditCmd_RemoveGameObject.h"

#include <filesystem>

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/InterOp/Helpers.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/DirectoryFunctions.h"
#include "AssetManager/Assets/Components/Light/DirectionallightComponent.h"
#include "AssetManager/Assets/Components/Light/PointlightComponent.h"
#include "AssetManager/Assets/Components/Light/SpotlightComponent.h"
#include "AssetManager/Assets/Components/Debugging/DebugDrawComponent.h"

#include <Timer.h>
#include <InputMapper.h>

ModelViewer::ModelViewer() : myModuleHandle(nullptr), myMainWindowHandle(nullptr), mySplashWindow(nullptr), mySettingsPath("Settings/mw_settings.json"), myApplicationState(), myLogger(), myCamera(), myGameObjects(), mySelectedObject(nullptr)
#ifndef _RETAIL
, myDebugMode(GraphicsEngine::DebugMode::Default), myLightMode(GraphicsEngine::LightMode::Default), myRenderMode(GraphicsEngine::RenderMode::Mesh), myIsShowingNewObjectWindow(true), myIsShowingPrefabWindow(true), myIsEditingPrefab(false), mySelectedPath(),
myNewObject(), mySelectedPrefabName(nullptr), myImguiNameCounts(), mySelectedComponentType(ComponentType::Mesh), myEditPrefab("Empty")
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
	InitImgui();

	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F5);
	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F6);
	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F7);
	input.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::F8);

	input.BindAction(CommonUtilities::eInputAction::Undo, CommonUtilities::KeyBind{ CommonUtilities::eKey::Z, CommonUtilities::eKey::Ctrl });
	input.BindAction(CommonUtilities::eInputAction::Redo, CommonUtilities::KeyBind{ CommonUtilities::eKey::Y, CommonUtilities::eKey::Ctrl });
	input.Attach(this, CommonUtilities::eInputAction::Undo);
	input.Attach(this, CommonUtilities::eInputAction::Redo);
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
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // _RETAIL

	return 0;
}

GameObject& ModelViewer::AddGameObject()
{
	GameObject newObject;
	auto iter = myGameObjects.emplace(newObject.GetID(), std::move(newObject));
	return iter.first->second;
}

GameObject& ModelViewer::AddGameObject(const GameObject& anObject)
{
	GameObject newObject(anObject);
	auto iter = myGameObjects.emplace(newObject.GetID(), std::move(newObject));
	return iter.first->second;
}

GameObject& ModelViewer::AddGameObject(GameObject&& anObject)
{
	auto iter = myGameObjects.emplace(anObject.GetID(), std::move(anObject));
	return iter.first->second;
}

GameObject* ModelViewer::GetGameObject(unsigned anID)
{
	assert(anID != 0 && "Incorrect ID!");

	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
		return &iter->second;
	}
	return nullptr;
}

bool ModelViewer::RemoveGameObject(unsigned anID)
{
	assert(anID != 0 && "Incorrect ID!");

	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
#ifndef _RETAIL
		if (mySelectedObject == &iter->second)
		{
			mySelectedObject = nullptr;
		}
#endif // !_RETAIL

		myGameObjects.erase(iter);
		return true;
	}
	return false;
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
	ShowWindow(myMainWindowHandle, SW_SHOW);
	SetForegroundWindow(myMainWindowHandle);
}

void ModelViewer::ModelViewer::SaveScene(const std::string& aPath) const
{
	std::string path = AddExtensionIfMissing(aPath, ".lvl");
	path = CreateValidPath(path, "Content/Scenes/", &myLogger);
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
		scene["GameObjects"][i] = object.ToJson();
		scene["GameObjects"][i].setComment("// GameObject ID: " + std::to_string(object.GetID()), Json::commentBefore);
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
	std::string path = AddExtensionIfMissing(aPath, ".lvl");
	path = GetValidPath(path, "Content/Scenes/", &myLogger);
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
		AddGameObject(object);
	}
}

void ModelViewer::Init()
{
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_SetAmbientlight>(nullptr, myApplicationState.AmbientIntensity));
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetShadowBias>(myApplicationState.ShadowBias));
	//LoadScene("Default");

	{
		auto& object = AddGameObject(AssetManager::GetAsset<GameObject>("Cube"));
		object.SetPosition({ 0.f, 0.f, 500.f });
		MeshComponent& mesh = object.GetComponent<MeshComponent>();
		//mesh.SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		//mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
		//mesh.SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Wooden_Carving_M.dds"));
		mesh.SetColor(GetColor(eColor::White));
		mesh.GetElements()[0].myMaterial.SetShininess(1000.f);
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset(Primitives::Pyramid));
		object.SetPosition({ 200.f, 0.f, 500.f });
		object.GetComponent<MeshComponent>().SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		object.GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
		object.GetComponent<MeshComponent>().SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Wooden_Carving_M.dds"));
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset(Primitives::Sphere));
		object.SetPosition({ -200.f, 0.f, 500.f });
		//object.GetComponent<MeshComponent>().SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		//object.GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
		//object.GetComponent<MeshComponent>().SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Wooden_Carving_M.dds"));
		object.GetComponent<MeshComponent>().SetColor(GetColor(eColor::White));
		object.GetComponent<MeshComponent>().GetElements()[0].myMaterial.SetShininess(1000.f);
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset(Primitives::InvertedCube));
		object.SetPosition({ 0.f, 0.f, 700.f });
		object.GetComponent<MeshComponent>().SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		object.GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
		object.GetComponent<MeshComponent>().SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Wooden_Carving_M.dds"));
		object.GetComponent<MeshComponent>().GetElements().back().myMaterial.SetUVTiling({2.f, 2.f});
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset<GameObject>("invertedpyramid"));
		object.SetPosition({ 200.f, 0.f, 700.f });
		object.GetComponent<MeshComponent>().SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		object.GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
		object.GetComponent<MeshComponent>().SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Wooden_Carving_M.dds"));
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset<GameObject>("invertedsphere"));
		object.SetPosition({ -200.f, 0.f, 700.f });
		object.GetComponent<MeshComponent>().SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		object.GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
		object.GetComponent<MeshComponent>().SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Wooden_Carving_M.dds"));
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset<GameObject>("SK_C_TGA_Bro"));
		object.SetPosition({ 200.f, 0.f, 200.f });
		//object.SetRotation({ 0.f, 180.f, 0.f });
		AnimatedMeshComponent& mesh = object.GetComponent<AnimatedMeshComponent>();
		mesh.SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/TGA_Bro_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/TGA_Bro_N.dds"));
		mesh.SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/TGA_Bro_M_Updated.dds"));
		mesh.SetAnimation(AssetManager::GetAsset<Animation>("Content/Animations/Idle/A_C_TGA_Bro_Idle_Wave.fbx"));
		mesh.StartAnimation();
		mesh.SetColor({ 1.f,0.f,0.f,.5f });
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset<GameObject>("Content/Models/SK_C_TGA_Bro.fbx"));
		object.SetPosition({ -200.f, 0.f, 200.f });
		//object.SetRotation({ 0.f, 180.f, 0.f });
		AnimatedMeshComponent& mesh = object.GetComponent<AnimatedMeshComponent>();
		mesh.SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/TGA_Bro_N.dds"));
		mesh.SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/TGA_Bro_M.dds"));
		mesh.SetAnimation(AssetManager::GetAsset<Animation>("Content/Animations/Idle/A_C_TGA_Bro_Idle_Brething.fbx"));
		mesh.SetLooping(true);
		mesh.StartAnimation();
		//mesh.SetColor({ 1.f,0.f,0.f,1.f });
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset<GameObject>("Content/Models/Chest.fbx"));
		object.SetPosition({ 0.f, 0.f, 200.f });
		//object.SetRotation({ 0.f, 180.f, 0.f });
		MeshComponent& mesh = object.GetComponent<MeshComponent>();
		mesh.SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Chest_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Chest_N.dds"));
		mesh.SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Chest_M.dds"));
		mesh.SetFXTexture(AssetManager::GetAsset<Texture*>("Content/Textures/FX/Chest_FX.dds"));
		for (auto& element : mesh.GetElements())
		{
			element.myMaterial.SetEmissionColor({ 1.f, 0.f, 0.f});
			element.myMaterial.SetEmissionIntensity(1.f);
		}		
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset<GameObject>("Content/Models/Buddha.fbx"));
		object.SetPosition({ 0.f, 200.f, 500.f });
		object.SetRotation({ 0.f, 180.f, 0.f });
		MeshComponent& mesh = object.GetComponent<MeshComponent>();
		mesh.SetAlbedoTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Buddha_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Buddha_N.dds"));
		mesh.SetMaterialTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Material/Buddha_M.dds"));
	}

	{
		auto& object = AddGameObject();
		object.SetPosition({ -100.f, 50.f, 400.f });

		PointlightComponent pointlight(200.f, 2.f, { 1.f, 0.f, 0.f });
		object.AddComponent(pointlight);

		/*DebugDrawComponent& debug = object.AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 200.f, true);*/
	}

	{
		auto& object = AddGameObject();
		object.SetPosition({ 100.f, 50.f, 400.f });

		PointlightComponent pointlight(200.f, 2.f, { 0.f, 1.f, 0.f });
		object.AddComponent(pointlight);

		/*DebugDrawComponent& debug = object.AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 200.f, true);*/
	}

	{
		auto& object = AddGameObject();
		object.SetPosition({ 0.f, 50.f, 600.f });

		PointlightComponent pointlight(200.f, 2.f, { 0.f, 0.f, 1.f });
		object.AddComponent(pointlight);

		/*DebugDrawComponent& debug = object.AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 200.f, true);*/
	}

	{
		auto& object = AddGameObject();
		object.SetPosition({ 0.f, 200.f, 600.f });

		SpotlightComponent spotlight(500, 2.f, 30.f, 50.f, { 0.f, -1.f, 1.f });
		object.AddComponent(spotlight);

		DebugDrawComponent& debug = object.AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 500.f, false, spotlight.GetLightDirection(), spotlight.GetLightDirection() + 0.1f, spotlight.GetLightDirection() - 0.1f);
	}

	{
		auto& object = AddGameObject();
		object.SetPosition({ 0.f, 200.f, 350.f });

		SpotlightComponent spotlight(500, 2.f, 30.f, 50.f, { 0.f, -1.f, -1.f });
		object.AddComponent(spotlight);

		DebugDrawComponent& debug = object.AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 500.f, false, spotlight.GetLightDirection(), spotlight.GetLightDirection() + 0.1f, spotlight.GetLightDirection() - 0.1f);
	}

	{
		auto& object = AddGameObject();
		DirectionallightComponent light({ 0.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, 1.f);
		object.AddComponent(light);
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset(Primitives::Plane));
		object.SetPosition({ 0.f, -10.f, 0.f });
		object.SetScale({ 20.f, 1.f, 20.f });
		object.GetComponent<MeshComponent>().SetColor({ 1.f, 1.f, 1.f, 1.f });
		//object.GetComponent<MeshComponent>().GetElements()[0].myMaterial.SetShininess(1.f);
	}

	{
		auto& object = AddGameObject(AssetManager::GetAsset(Primitives::Plane));
		object.SetPosition({ 0.f, 240.f, 1000.f });
		object.SetScale({ 20.f, 1.f, 5.f });
		object.SetRotation({ 90.f, 0.f, 0.f });
		object.GetComponent<MeshComponent>().SetColor({ 1.f, 1.f, 1.f, 1.f });
		//object.GetComponent<MeshComponent>().SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Default/UV_checker_Map.dds"));
	}

	//SaveScene("Default");
}

void ModelViewer::Update()
{
	GraphicsEngine& engine = GraphicsEngine::Get();
	engine.BeginFrame();
	CommonUtilities::Timer::Update();
	CommonUtilities::InputMapper::GetInstance()->Notify();

#ifndef _RETAIL
	UpdateImgui();
#endif // _RETAIL

	myCamera.Update();
	UpdateScene();

	CommonUtilities::InputMapper::GetInstance()->Update();
	engine.RenderFrame();

#ifndef _RETAIL
	RenderImgui();
#endif // _RETAIL

	if (myIncommingCommands.size() > 0)
	{
		myRedoCommands.clear();
	}

	for (auto& command : myIncommingCommands)
	{
		command->Execute();
		myUndoCommands.emplace_back(command);
	}
	myIncommingCommands.clear();

	engine.EndFrame();
}

void ModelViewer::UpdateScene()
{
	for (auto& [id, object] : myGameObjects)
	{
		object.Update();
	}
}

#ifndef _RETAIL
void ModelViewer::ModelViewer::InitImgui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(myMainWindowHandle);
	ImGui_ImplDX11_Init(RHI::Device.Get(), RHI::Context.Get());
}

void ModelViewer::ModelViewer::UpdateImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
	CreatePreferenceWindow();
	CreateSceneContentWindow();
	CreateSelectedObjectWindow();
	CreatePrefabWindow();
	CreateNewObjectWindow();
}

void ModelViewer::ModelViewer::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ModelViewer::CreatePreferenceWindow()
{
	if (ImGui::Begin("Preferences"))
	{
		ImGui::SeparatorText("Launch Settings");
		ImGui::Checkbox("Start Maximized", &myApplicationState.StartMaximized);
		ImGui::DragInt2("Window Size", &myApplicationState.WindowSize.x, 1.f, 0, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::InputText("Window Title", &myApplicationState.WindowTitle);

		ImGui::SeparatorText("Camera Settings");
		if (ImGui::DragFloat("Movement Speed", &myApplicationState.CameraSpeed))
		{
			myCamera.SetMovementSpeed(myApplicationState.CameraSpeed);
		}
		if (ImGui::DragFloat("Rotation Speed", &myApplicationState.CameraRotationSpeed))
		{
			myCamera.SetRotationSpeed(myApplicationState.CameraRotationSpeed);
		}
		if (ImGui::DragFloat("Mouse Sensitivity", &myApplicationState.CameraMouseSensitivity))
		{
			myCamera.SetMouseSensitivity(myApplicationState.CameraMouseSensitivity);
		}

	ImGui::SeparatorText("Scene Settings");
	if (ImGui::DragFloat("Ambientlight Intensity", &myApplicationState.AmbientIntensity, 0.01f, 0.f, INFINITY, "%.3f", ImGuiSliderFlags_AlwaysClamp))
	{
		GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetAmbientlight>(nullptr, myApplicationState.AmbientIntensity));
	}
	if (ImGui::DragFloat("Shadow Bias", &myApplicationState.ShadowBias, 0.0001f, 0.f, 1.f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
	{
		GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetShadowBias>(myApplicationState.ShadowBias));
	}

		ImGui::SeparatorText("");
		if (ImGui::Button("Save Preferences"))
		{
			SaveState();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save Scene"))
		{
			// Add file selector
			SaveScene("Default");
		}
	}
	ImGui::End();
}

void ModelViewer::CreateSelectedObjectWindow()
{
	if (ImGui::Begin("Selected GameObject"))
	{
		if (mySelectedObject)
		{
			mySelectedObject->CreateImGuiWindowContent("Selected GameObject");

			ImGui::Separator();
			if (ImGui::Button("Save As Prefab"))
			{
				ImGui::OpenPopup("Select Prefab Name");
			}
			if (ImGui::BeginPopupModal("Select Prefab Name"))
			{
				static std::string name;
				if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					AssetManager::SavePrefab(*mySelectedObject, name);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Save"))
				{
					AssetManager::SavePrefab(*mySelectedObject, name);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}		
	}
	ImGui::End();
}

void ModelViewer::CreateSceneContentWindow()
{
	myImguiNameCounts.clear();

	if (ImGui::Begin("Scene"))
	{
		for (auto& [id, object] : myGameObjects)
		{
			if (auto iter = myImguiNameCounts.find(object.GetName()); iter != myImguiNameCounts.end())
			{
				std::string text = object.GetName() + " (" + std::to_string(iter->second++) + ")";
				if (ImGui::Button(text.c_str()))
				{
					mySelectedObject = &object;
				}
			}
			else
			{
				myImguiNameCounts.emplace(object.GetName(), 1);
				if (ImGui::Button(object.GetName().c_str()))
				{
					mySelectedObject = &object;
				}
			}
		}
	}
	ImGui::End();
}

void ModelViewer::CreatePrefabWindow()
{
	if (ImGui::Begin("Prefab", &myIsShowingPrefabWindow))
	{
		if (ImGui::BeginCombo(" ", mySelectedPrefabName ? mySelectedPrefabName->c_str() : ""))
		{
			for (auto& path : AssetManager::GetAvailablePrefabs())
			{
				const bool isSelected = (mySelectedPrefabName == &path);
				if (ImGui::Selectable(path.c_str(), isSelected))
				{
					mySelectedPrefabName = &path;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (mySelectedPrefabName)
		{
			if (ImGui::Button("Set as new object"))
			{
				myNewObject = AssetManager::GetPrefab(*mySelectedPrefabName);
				myNewObject.MarkAsPrefab();
				if (!myIsShowingNewObjectWindow)
				{
					myIsShowingNewObjectWindow = true;
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Add to Scene"))
			{
				AddCommand(std::make_shared<EditCmd_AddGameObject>(AssetManager::GetPrefab(*mySelectedPrefabName)));
			}
		}
		
		ImGui::Separator();

		if (ImGui::Button("Select Prefab"))
		{
			if (ShowFileSelector(mySelectedPath, { L"Prefab files", ToWString("*" + AssetManager::GetPrefabExtension()) }, ToWString(GetFullPath(AssetManager::GetPrefabPath())), L"Select Prefab"))
			{
				myLogger.Log("Selected File!");
			}
		}
		ImGui::Text(GetRelativePath(mySelectedPath).c_str());
	}
	ImGui::End();
}

void ModelViewer::CreateNewObjectWindow()
{
	if (ImGui::Begin("New Object", &myIsShowingNewObjectWindow))
	{
		if (ImGui::Button("Reset to Default"))
		{
			myNewObject = AssetManager::GetPrefab("Default");
		}
		if (ImGui::BeginCombo(" ", ComponentTypeToString(mySelectedComponentType).c_str()))
		{
			int* selectedType = (int*)&mySelectedComponentType;
			for (int i = 1; i < static_cast<int>(ComponentType::Count); i++)
			{
				const bool isSelected = (*selectedType == i);
				if (ImGui::Selectable(ComponentTypeToString(static_cast<ComponentType>(i)).c_str(), isSelected))
				{
					*selectedType = i;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Component"))
		{
			AddComponent(mySelectedComponentType, myNewObject);
		}

		ImGui::Separator();
		myNewObject.CreateImGuiWindowContent("New Object");
		ImGui::Separator();

		if (ImGui::Button("Add To Scene"))
		{
			AddCommand(std::make_shared<EditCmd_AddGameObject>(myNewObject));
		}
		ImGui::SameLine();
		if (ImGui::Button("Save As Prefab"))
		{
			ImGui::OpenPopup("Select Prefab Name");
		}
		if (ImGui::BeginPopupModal("Select Prefab Name"))
		{
			static std::string name;
			if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				AssetManager::SavePrefab(myNewObject, name);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				AssetManager::SavePrefab(myNewObject, name);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void ModelViewer::AddCommand(const std::shared_ptr<EditCommand>& aCommand)
{
	// Potential future problem with using infinite Undo-stack
	myIncommingCommands.emplace_back(aCommand);
}

void ModelViewer::Undo()
{
	if (myUndoCommands.size() > 0)
	{
		myUndoCommands.back()->Undo();
		myRedoCommands.emplace_back(myUndoCommands.back());
		myUndoCommands.pop_back();
	}	
}

void ModelViewer::Redo()
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
		Undo();
	}
	else if(anAction == CommonUtilities::eInputAction::Redo)
	{
		Redo();
	}
}
#endif // _RETAIL
