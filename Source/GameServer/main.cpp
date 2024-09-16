#include "GameServer.h"

GameServer globalServer;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
		globalServer.Shutdown();
		return TRUE;

	default:
		return FALSE;
	}
}

int main()
{
	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	if (globalServer.Initialize())
	{
		return globalServer.Run();
	}
	return EXIT_SUCCESS;
}