#pragma once
#define WIN32_LEAN_AND_MEAN
#include <vector>
#include <thread>
#include <WS2tcpip.h>
#include <mutex>
#include "NetworkShared/NetMessage.h"
#include "Logging/FileLogger.h"

namespace Network
{
	class Client
	{
		friend class NetworkManager;
	public:
		Client();
		~Client();

		void Init();

		bool SendNetMessage(const NetMessage& aMessage);

		std::vector<NetMessage> Flush();

		void Recieve();

		bool IsConnected() const;
		bool Connect();
		void Disconnect();

	private:
		std::vector<NetMessage> myMessages;
		WSADATA myWSA;
		FileLogger myLogger;
		std::mutex myMutex;
		std::string myLastError;
		sockaddr_in myServer;
		SOCKET mySocket;
		std::thread* myThread;

		unsigned myFailedMessageCount;

		bool myHasError;
		bool myIsRunning;
		bool myIsConnected;
		bool myIsInitialized;
		bool myServerDisconnected;

		void CheckError();
	};
}