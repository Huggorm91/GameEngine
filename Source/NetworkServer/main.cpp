#include "Server.h"

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

int main()
{
	globalServer = new Network::Server();
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	globalServer->Init();
	while (globalServer->IsRunning())
	{
		globalServer->Update();
	}

	globalServer->ShutDown();
	system("PAUSE");
	return 0;
}