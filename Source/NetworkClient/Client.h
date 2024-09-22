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

		// messageID needs to be manually set before sending the NetMessage to this function
		bool SendMultipartMessage(const NetMessage& aMessage);
		unsigned short GetMessageID();

		std::vector<NetMessage> Flush();

		void Recieve();

		bool IsConnected() const;
		bool Connect();
		void Disconnect();

	private:
		std::vector<NetMessage> myMessages;
		std::vector<NetMessage> myMultipartMessages;

		WSADATA myWSA;
		FileLogger myLogger;
		std::mutex myMutex;
		std::string myLastError;
		sockaddr_in myServer;
		SOCKET mySocket; // TODO: Create 2 sockets, one for sending and one for recieving
		std::thread* myThread;
		
		unsigned myFailedMessageCount;

		unsigned short mySenderID;
		unsigned short myIDGenerator;		

		bool myHasError;
		bool myIsRunning;
		bool myIsConnected;
		bool myIsInitialized;
		bool myServerDisconnected;

		void HandleMultiMessages();

		// This logs the last error reported my the thread running Recieve()
		void LogLastError();

		bool CanSendMessage();
		bool SendNetMessageInternal(const NetMessage& aMessage);
	};
}