#include "GameLauncher.pch.h"
#include "GameLauncher.h"
#include "resource.h"

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "AssetManager/AssetManager.h"

#include "NetworkClient/MessageHandler.h"
#include "NetworkShared/MessageFunctions.h"

#include "CrimsonUtilities/Time/Time.h"
#include "CrimsonUtilities/Json/jsonCpp/json.h"
#include "CrimsonUtilities/File/DirectoryFunctions.h"

#include "GameplayEngine/Engine.h"
#include "GameplayEngine/Input/InputMapper.h"
#include "GameplayEngine/Managers/SceneManager.h"
#include "GameplayEngine/Managers/ObjectManager.h"

constexpr const char* globalSettingsPath("Data/settings.json");

GameLauncher::GameLauncher() :
	myModuleHandle(nullptr),
	myMainWindowHandle(nullptr)
{
}

bool GameLauncher::Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess)
{
	myLogger = Logger::Create("GameLauncher");
	myModuleHandle = aHInstance;
	LoadSettings();

	myLogger.Log(Crimson::GetAppPath());

	constexpr LPCWSTR windowClassName = L"GameLauncher_MainWindow";

	// First we create our Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = aWindowProcess;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = windowClassName;
	windowClass.hIcon = LoadIcon(aHInstance, MAKEINTRESOURCE(IDI_GAMELAUNCHER));
	RegisterClass(&windowClass);

	std::wstring stdTitle{ Crimson::ToWString(mySettings.windowTitle) };
	LPCWSTR title{ stdTitle.c_str() };

	DWORD flags;
	if (mySettings.isMaximized)
	{
		flags = WS_OVERLAPPEDWINDOW | WS_POPUP | WS_MAXIMIZE;
	}
	else
	{
		flags = WS_OVERLAPPEDWINDOW | WS_POPUP;
	}

	// Get center of screen
	RECT windowRect{};
	SystemParametersInfo(SPI_GETWORKAREA, 0, &windowRect, 0);

	if (mySettings.windowSize == Crimson::Vector2i::Null)
	{
		mySettings.windowSize = { 1920, 1080 };
	}

	windowRect.left = static_cast<LONG>((windowRect.right * 0.5f) - (mySettings.windowSize.x * 0.5f));
	windowRect.top = static_cast<LONG>((windowRect.bottom * 0.5f) - (mySettings.windowSize.y * 0.5f));
	windowRect.right = mySettings.windowSize.x;
	windowRect.bottom = mySettings.windowSize.y;

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

#ifndef _DEBUG
	try
	{
#endif // _DEBUG

		Engine::Init(myMainWindowHandle, mySettings.windowSize);

#ifdef _RETAIL
		GraphicsEngine::Get().Initialize(myMainWindowHandle, false);
#else
		GraphicsEngine::Get().Initialize(myMainWindowHandle, true);
#endif // _RETAIL

		AssetManager::Init();
		AssetManager::GeneratePrimitives();

		myMessageHandler.Init();

#ifndef _DEBUG
	}
	catch (const std::exception& anException)
	{
		HandleCrash(anException);
		Shutdown();
	}
	catch (...)
	{
		HandleCrash(std::invalid_argument("Caught unknown Error!"));
		Shutdown();
	}
#endif // _DEBUG

	if (mySettings.isMaximized)
	{
		ShowWindow(myMainWindowHandle, SW_MAXIMIZE);
	}
	else
	{
		ShowWindow(myMainWindowHandle, SW_SHOW);
	}
	SetForegroundWindow(myMainWindowHandle);

	return true;
}

int GameLauncher::Run()
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
			HandleCrash(anException);
		}
		catch (...)
		{
			isRunning = false;
			HandleCrash(std::invalid_argument("Caught unknown Error!"));
		}
#endif // _DEBUG
	}

	Shutdown();

	return 0;
}

void GameLauncher::Shutdown()
{
}

void GameLauncher::HandleCrash(const std::exception& anException)
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
	myLogger.SetPrintToFile(true, "CrashReports\\" + Crimson::FileNameTimestamp() + "_Log.txt");
	myLogger.LogException(anException);

	// Leave console up to let user read information
	system("PAUSE");
}

void GameLauncher::LoadSettings()
{
	std::fstream fileStream(globalSettingsPath, std::ios::in);
	if (fileStream)
	{
		Json::Value json;
		fileStream >> json;
		fileStream.flush();
		mySettings = json;
	}
	else
	{
		myLogger.Err("Could not load settings!");
	}
}

void GameLauncher::Init()
{
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_SetAmbientlight>(nullptr, mySettings.ambientIntensity));
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetShadowBias>(mySettings.shadowBias));

	Engine::GetSceneManager().LoadScene("Test");
}

void GameLauncher::Update()
{
	GraphicsEngine& graphicsEngine = GraphicsEngine::Get();
	graphicsEngine.BeginFrame();
	Engine::BeginFrame();
	myMessageHandler.Update();

	Engine::GetObjectManager().UpdateObjects();

	Engine::EndFrame();
	graphicsEngine.RenderFrame();
	graphicsEngine.EndFrame();
}

void GameLauncher::HandleNetmessages()
{
	auto& messages = myMessageHandler.GetMessages();
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
