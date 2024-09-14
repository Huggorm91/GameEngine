#include "NetworkServer/Server.h"
#include "GameplayEngine/Engine.h"
#include "AssetManager/AssetManager.h"
#include "GraphicsEngine/InterOp/RHI.h"

inline Network::Server* globalServer = nullptr;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
		delete globalServer;
		return TRUE;

	default:
		return FALSE;
	}
}

void HandleCrash(const std::exception& anException)
{
	// Log crash
	//myLogger.Err("Server has crashed!");
	//myLogger.Warn("Writing exception to log file!");
	//myLogger.LogException(anException);
	//Engine::GetLogger().PrintHistoryToFile("CrashReports\\" + Crimson::FileNameTimestamp() + "_Log.txt");

	// Leave console up to let user read information
	system("PAUSE");
}

int main()
{
	globalServer = new Network::Server();
	SetConsoleCtrlHandler(CtrlHandler, TRUE);

#ifndef _DEBUG
	try
	{
#endif // _DEBUG
		Engine::Init(NULL, Crimson::Vector2i::Null, false);

		AssetManager::Init();
		AssetManager::GeneratePrimitives();

		globalServer->Init();
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
	while (globalServer->IsRunning())
	{
		globalServer->Update();
	}

	globalServer->ShutDown();
	system("PAUSE");
	return 0;
}