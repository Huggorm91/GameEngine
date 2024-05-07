#pragma once
#include "Client.h"
#include <unordered_map>

class Server
{
public:
	Server() = default;
	~Server() = default;

	void Init();
	void Update();

	bool IsRunning() const;

	void ShutDown();

private:
	struct Client
	{
		sockaddr_in socket;
		std::string username;
		std::string ip;
		unsigned short port = 0;

		inline bool operator==(const Client& aClient) const
		{
			return aClient.port == port && aClient.ip == ip;
		}
	};

	std::unordered_map<std::string, Client> myClients;
	NetMessage myMessage;
	WSADATA myWSA;
	Logger myLogger;
	sockaddr_in myClientInfo;
	sockaddr_in myServerInfo;
	SOCKET myServerSocket;

	int mySocketSize;

	bool myIsRunning;

	void ErrorShutDown();
};

