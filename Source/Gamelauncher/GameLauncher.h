#pragma once
#include "GameSettings.h"
#include "Logging/Logging.h"
#include "NetworkClient/MessageHandler.h"

class GameLauncher
{
public:
	GameLauncher();
	~GameLauncher() = default;

	bool Initialize(HINSTANCE aHInstance, WNDPROC aWindowProcess);
	int Run();
	void Shutdown();

private:
	HINSTANCE myModuleHandle;
	HWND myMainWindowHandle;
	GameSettings mySettings;
	Network::MessageHandler myMessageHandler;
	Logger myLogger;

	void HandleCrash(const std::exception& anException);

	void LoadSettings();

	void Init();
	void Update();

	void HandleNetmessages();
};