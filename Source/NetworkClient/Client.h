#pragma once
#define WIN32_LEAN_AND_MEAN
#include <vector>
#include <thread>
#include <WS2tcpip.h>
#include <mutex>
#include "NetworkShared/NetMessage.h"
#include "Logging/Logging.h"

namespace Network
{
	class Client
	{
		friend class MessageHandler;
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
		Logger myLogger;
		std::mutex myMutex;
		sockaddr_in myServer;
		SOCKET mySocket;
		std::thread* myThread;

		unsigned myFailedMessageCount;

		bool myIsRunning;
		bool myIsConnected;
		bool myIsInitialized;
		bool myServerDisconnected;
	};
}