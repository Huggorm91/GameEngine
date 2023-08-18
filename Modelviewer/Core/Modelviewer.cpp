
// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "Modelviewer.h"
#include "Windows/SplashWindow.h"

#include <filesystem>

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/InterOp/Helpers.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetDirectionallight.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/Assets/Components/Light/DirectionallightComponent.h"
#include "AssetManager/Assets/Components/Light/PointlightComponent.h"
#include "AssetManager/Assets/Components/Light/SpotlightComponent.h"
#include "AssetManager/Assets/Components/Debugging/DebugDrawComponent.h"

#include <Timer.h>
#include <InputMapper.h>

bool ModelViewer::Initialize(HINSTANCE aHInstance, SIZE aWindowSize, WNDPROC aWindowProcess, LPCWSTR aWindowTitle)
{
	myLogger = Logger::Create("ModelViewer");

	myModuleHandle = aHInstance;

	constexpr LPCWSTR windowClassName = L"ModelViewerMainWindow";

	// First we create our Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = aWindowProcess;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = windowClassName;
	RegisterClass(&windowClass);

	// Get center of screen
	RECT windowRect;
	GetClientRect(GetDesktopWindow(), &windowRect);
	windowRect.left = static_cast<LONG>((windowRect.right * 0.5f) - (aWindowSize.cx * 0.5f));
	windowRect.top = static_cast<LONG>((windowRect.bottom * 0.5f) - (aWindowSize.cy * 0.5f));

	// Then we use the class to create our window
	myMainWindowHandle = CreateWindow(
		windowClassName,                                // Classname
		aWindowTitle,                                    // Window Title
		WS_OVERLAPPEDWINDOW | WS_POPUP,    // Flags
		windowRect.left,
		windowRect.top,
		aWindowSize.cx,
		aWindowSize.cy,
		nullptr, nullptr, nullptr,
		nullptr
	);

	ShowSplashScreen();

	// TODO: Load all setting from json
	CommonUtilities::InputMapper::GetInstance()->Init(myMainWindowHandle);
	AssetManager::Init();
#ifdef _RETAIL
	GraphicsEngine::Get().Initialize(myMainWindowHandle, false);
#else
	GraphicsEngine::Get().Initialize(myMainWindowHandle, true);
#endif // _RETAIL	
	myCamera.Init({ static_cast<float>(aWindowSize.cx), static_cast<float>(aWindowSize.cy) });
	AssetManager::GeneratePrimitives();

	HideSplashScreen();

	return true;
}

int ModelViewer::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

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

	return 0;
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

void ModelViewer::Init()
{
	myGameObjects.emplace_back(AssetManager::GetAsset<GameObject>("Cube"));
	myGameObjects.back().SetPosition({ 0.f, 0.f, 500.f });
	{
		MeshComponent& mesh = myGameObjects.back().GetComponent<MeshComponent>();
		mesh.SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
	}
	
	myGameObjects.emplace_back(AssetManager::GetAsset(Primitives::Pyramid));
	myGameObjects.back().SetPosition({ 200.f, 0.f, 500.f });
	myGameObjects.back().GetComponent<MeshComponent>().SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
	myGameObjects.back().GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));
	
	myGameObjects.emplace_back(AssetManager::GetAsset(Primitives::Sphere));
	myGameObjects.back().SetPosition({ -200.f, 0.f, 500.f });
	myGameObjects.back().GetComponent<MeshComponent>().SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
	myGameObjects.back().GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));

	myGameObjects.emplace_back(AssetManager::GetAsset(Primitives::InvertedCube));
	myGameObjects.back().SetPosition({ 0.f, 0.f, 700.f });
	myGameObjects.back().GetComponent<MeshComponent>().SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
	myGameObjects.back().GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));

	myGameObjects.emplace_back(AssetManager::GetAsset<GameObject>("invertedpyramid"));
	myGameObjects.back().SetPosition({ 200.f, 0.f, 700.f });
	myGameObjects.back().GetComponent<MeshComponent>().SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
	myGameObjects.back().GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));

	myGameObjects.emplace_back(AssetManager::GetAsset<GameObject>("invertedsphere"));
	myGameObjects.back().SetPosition({ -200.f, 0.f, 700.f });
	myGameObjects.back().GetComponent<MeshComponent>().SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Wooden_Carving_C.dds"));
	myGameObjects.back().GetComponent<MeshComponent>().SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Wooden_Carving_N.dds"));

	myGameObjects.emplace_back(AssetManager::GetAsset<GameObject>("Content/Models/SK_C_TGA_Bro.fbx"));
	myGameObjects.back().SetPosition({ 0.f, 0.f, 200.f });
	//myModels.back().SetRotation({ 0.f, 180.f, 0.f });
	{
		AnimatedMeshComponent& mesh = myGameObjects.back().GetComponent<AnimatedMeshComponent>();
		mesh.SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/TGA_Bro_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/TGA_Bro_N.dds"));
		mesh.SetAnimation(AssetManager::GetAsset<Animation&>("Content/Animations/Idle/A_C_TGA_Bro_Idle_Wave.fbx"));
		mesh.StartAnimation();
		mesh.SetColor({ 1.f,0.f,0.f,.5f });
	}

	myGameObjects.emplace_back(AssetManager::GetAsset<GameObject>("Content/Models/SK_C_TGA_Bro.fbx"));
	myGameObjects.back().SetPosition({ 100.f, 0.f, 200.f });
	//myModels.back().SetRotation({ 0.f, 180.f, 0.f });
	{
		AnimatedMeshComponent& mesh = myGameObjects.back().GetComponent<AnimatedMeshComponent>();
		mesh.SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/TGA_Bro_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/TGA_Bro_N.dds"));
		mesh.SetAnimation(AssetManager::GetAsset<Animation&>("Content/Animations/Idle/A_C_TGA_Bro_Idle_Brething.fbx"));
		mesh.StartAnimation(true);
		//mesh.SetColor({ 1.f,0.f,0.f,1.f });
	}

	myGameObjects.emplace_back(AssetManager::GetAsset<GameObject>("Content/Models/Chest.fbx"));
	myGameObjects.back().SetPosition({ -200.f, 0.f, 200.f });
	//myModels.back().SetRotation({ 0.f, 180.f, 0.f });
	{
		MeshComponent& mesh = myGameObjects.back().GetComponent<MeshComponent>();
		mesh.SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Albedo/Chest_C.dds"));
		mesh.SetNormalTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Normal/Chest_N.dds"));
	}

	myGameObjects.emplace_back();
	{
		myGameObjects.back().SetPosition({ -100.f, 50.f, 500.f });

		PointlightComponent pointlight(200.f, 1.f, CommonUtilities::Vector3f::Null, { 1.f, 0.f, 0.f });
		myGameObjects.back().AddComponent(pointlight);

		DebugDrawComponent& debug = myGameObjects.back().AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 200.f, true);
	}

	myGameObjects.emplace_back();
	{
		myGameObjects.back().SetPosition({ 100.f, 50.f, 500.f });

		PointlightComponent pointlight(200.f, 1.f, CommonUtilities::Vector3f::Null, { 0.f, 1.f, 0.f });
		myGameObjects.back().AddComponent(pointlight);

		DebugDrawComponent& debug = myGameObjects.back().AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 200.f, true);
	}

	myGameObjects.emplace_back();
	{
		myGameObjects.back().SetPosition({ 0.f, 50.f, 600.f });

		PointlightComponent pointlight(200.f, 1.f, CommonUtilities::Vector3f::Null, { 0.f, 0.f, 1.f });
		myGameObjects.back().AddComponent(pointlight);

		DebugDrawComponent& debug = myGameObjects.back().AddComponent<DebugDrawComponent>();
		debug.SetAxisLines(CommonUtilities::Vector3f::Null, 200.f, true);
	}

	/*myGameObjects.emplace_back();
	{
		DirectionallightComponent light(CommonUtilities::Vector3f{ 0.f, -1.f, -1.f });
		myGameObjects.back().AddComponent(light);
	}*/

	myGameObjects.emplace_back(AssetManager::GetAsset(Primitives::Plane));
	myGameObjects.back().SetPosition({ 0.f, -1.f, 0.f });
	myGameObjects.back().SetScale({ 20.f, 20.f, 20.f });
	myGameObjects.back().GetComponent<MeshComponent>().SetColor({ 1.f, 1.f, 1.f, 1.f });
	//myGameObjects.back().GetComponent<MeshComponent>().SetTexture(AssetManager::GetAsset<Texture*>("Content/Textures/Default/UV_checker_Map.dds"));
}

void ModelViewer::Update()
{
	GraphicsEngine& engine = GraphicsEngine::Get();
	engine.BeginFrame();
	CommonUtilities::Timer::Update();
	CommonUtilities::InputMapper::GetInstance()->Notify();

	myCamera.Update();
	engine.AddGraphicsCommand(std::make_shared<LitCmd_SetDirectionallight>(CommonUtilities::Vector3f{ 0.f, -1.f, 0.f }, CommonUtilities::Vector3f{ 0.225f, 0.225f , 0.225f }));
	UpdateScene();

	CommonUtilities::InputMapper::GetInstance()->Update();
	engine.RenderFrame();
	engine.EndFrame();
}

void ModelViewer::UpdateScene()
{
	for (auto& model : myGameObjects)
	{
		model.Update();
	}
}
