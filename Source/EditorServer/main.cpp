#include "NetworkServer/Server.h"

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
		auto messages = globalServer->Flush();
		// This list now contains all incomming messages since Flush was last called
		Sleep(1000); // Sleep for a second to not flush too often, since it locks the mutex preventing the server to handle other traffic
	}

	globalServer->ShutDown();
	system("PAUSE");
	return 0;
}