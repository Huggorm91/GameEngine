#pragma once
#include <vector>
#include <thread>
#include <WS2tcpip.h>
#include <mutex>
#include "Shared\NetMessage.h"
#include "Logging/Logging.h"

class Client
{
public:
	Client();
	~Client();

	void Init();
	void Update();

	bool SendNetMessage(const NetMessage& aMessage);

	std::vector<NetMessage> Flush();

	void Recieve();

	bool IsConnected() const;

private:
	std::vector<NetMessage> myMessages;
	Logger myLogger;
	std::mutex myMutex;
	sockaddr_in myServer;
	SOCKET mySocket;
	std::thread* myThread;

	unsigned myFailedMessageCount;

	bool myIsRunning;
	bool myIsConnected;
	bool myIsInitialized;

	bool Connect();
	void Disconnect();
};

