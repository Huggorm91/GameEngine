#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#include "NetworkShared/NetMessage.h"
#include "Logging/MainLogger.h"
#include <unordered_map>

namespace Network
{
	class Server
	{
	public:
		Server();
		~Server();

		void Init();
		void Update();

		bool IsRunning() const;

		void ShutDown();

	private:
		struct ClientInfo
		{
			sockaddr_in socket {};
			std::string username;
			std::string ip;
			unsigned short port = 0;
			unsigned short failedMessageCount = 0;

			inline bool operator==(const ClientInfo& aClient) const
			{
				return aClient.port == port && aClient.ip == ip;
			}
		};

		std::unordered_map<std::string, ClientInfo> myClients;
		std::unordered_map<std::string, unsigned short> myClientIDs;
		std::vector<std::string> myRemovedClients;
		NetMessage myMessage;
		WSADATA myWSA;
		MainLogger myLogger;
		sockaddr_in myClientInfo;
		sockaddr_in myServerInfo;
		SOCKET myServerSocket;
		PSTR myCurrentIP;

		int mySocketSize;
		unsigned short myIDGenerator; // Very Hightech!

		bool myIsRunning;

		void ErrorShutDown();

		void HandleConnect(ClientInfo& outClient, const std::string& anIdentifier);
		void HandleDisconnect(const ClientInfo& aClient, const std::string& anIdentifier);
		void HandleConfirmation();
		void HandlePing(const std::string& anIdentifier);
		void HandleChat(const ClientInfo& aClient, const std::string& anIdentifier);
		void HandleGameObjectMessage(const ClientInfo& aClient, const std::string& anIdentifier);

		void SetMessageData(const std::string& aMessage);
		void SendMessageToClients(ClientInfo* aCurrentClient);
		void SendToClient(ClientInfo& outClient, const std::string& anIdentifier);

		std::string GetIdentifier(char* anIP, unsigned short aPort);
	};
}