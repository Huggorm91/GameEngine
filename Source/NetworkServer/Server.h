#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#include "NetworkShared/NetMessage.h"
#include "Logging/MainLogger.h"
#include <unordered_map>
#include <functional>
#include <shared_mutex>

namespace Network
{
	struct ClientInfo
	{
		sockaddr_in socket{};
		std::string username;
		std::string ip;
		unsigned short port = 0;
		unsigned short failedMessageCount = 0;

		inline bool operator==(const ClientInfo& aClient) const
		{
			return aClient.port == port && aClient.ip == ip;
		}
	};

	class Server
	{
	public:
		Server();
		~Server();

		void Init(MainLogger* aLogger = nullptr);
		void Update();

		bool IsRunning() const;

		void ShutDown();

		void SetConnectionCallback(const std::function<void(ClientInfo&)>& aFunction);

		// If aClientToAvoid is nullptr this function will send to all connected clients.
		void SendMessageToClients(const NetMessage& aMessage, ClientInfo* aClientToAvoid = nullptr);
		void SendToClient(const NetMessage& aMessage, ClientInfo& outClient);

		unsigned short GetMessageID();

		static std::string GetLogfileName();

	private:
		NetMessage myMessage;
		WSADATA myWSA;
		std::unordered_map<std::string, ClientInfo> myClients;
		std::unordered_map<std::string, unsigned short> myClientIDs;
		std::unordered_map<std::string, std::vector<NetMessage>> myClientHistory;
		std::unique_ptr<MainLogger, std::function<void(MainLogger*)>> myLogger;
		std::function<void(ClientInfo&)> myConnectCallback;
		std::vector<std::string> myRemovedClients;
		sockaddr_in myClientInfo;
		sockaddr_in myServerInfo;
		SOCKET myServerSocket;
		PSTR myCurrentIP;

		int mySocketSize;
		unsigned short myClientIDGenerator; // Very Hightech!
		unsigned short myMessageIDGenerator; // Very Hightech!

		bool myIsRunning;

		void ErrorShutDown();

		void HandleConnect(ClientInfo& outClient, const std::string& anIdentifier);
		void HandleDisconnect(const ClientInfo& aClient, const std::string& anIdentifier);
		void HandleConfirmation();
		void HandlePing(const std::string& anIdentifier);
		void HandleChat(const ClientInfo& aClient, const std::string& anIdentifier);
		void HandleGameObjectMessage(const ClientInfo& aClient, const std::string& anIdentifier);

		void SetMessageData(const std::string& aMessage);

		std::string GetIdentifier(const char* anIP, unsigned short aPort);

		// Returns the error code
		int LogWSAError();
	};
}