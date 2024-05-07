#include "Server.h"

int main()
{
	Server server;
	server.Init();
	while (server.IsRunning())
	{
		server.Update();
	}

	server.ShutDown();

	return 0;
}