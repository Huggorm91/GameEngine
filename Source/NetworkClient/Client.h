#pragma once
#include <vector>
#include <thread>
#include <WS2tcpip.h>
#include <mutex>
#include "Shared\NetMessage.h"

class Client
{
public:
	Client();
	~Client();

	void Init();
	void Update();

	void SendNetMessage(const NetMessage& aMessage);

	std::vector<NetMessage> Flush();

	void Recieve();

	bool IsConnected() const;

private:
	std::vector<NetMessage> myMessages;
	std::mutex myMutex;
	std::thread* myThread;
	sockaddr_in myServer;
	SOCKET mySocket;
	bool myIsRunning;
	bool myIsConnected;
	bool myIsInitialized;

	bool Connect();
};

