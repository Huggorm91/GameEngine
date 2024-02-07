#include"Client.h"

inline Client* globalClient = nullptr;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
		delete globalClient;
		return TRUE;

	default:
		return FALSE;
	}
}

int main()
{
	system("title UDP Client");
	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	globalClient = new Client();
	globalClient->Init();
	while (true)
	{
		globalClient->Update();
	}

	return 0;
}