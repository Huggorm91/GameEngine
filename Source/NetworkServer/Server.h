#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#include "NetworkShared/NetMessage.h"
#include "Logging/MainLogger.h"
#include <array>
#include <functional>
#include <unordered_map>

namespace Network
{
	struct ConfirmationData;

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

		std::vector<NetMessage> Flush(float aTimeSinceLastFlushInSeconds);
		void ReportStatistics();

		void SetTimeBetweenResend(float aTimeInSeconds);
		void SetMaximumResendAttempts(uint8_t anAmount);

		// This is run on a separate thread, should not be called by the program
		void Update();

		bool IsRunning() const;

		void ShutDown();

		void SetIncommingMessageCallback(MessageType aType, const std::function<void(ClientInfo&, const NetMessage&)>& aFunction);

		// If aClientToAvoid is nullptr this function will send to all connected clients.
		void SendMessageToClients(const NetMessage& aMessage, ClientInfo* aClientToAvoid = nullptr);
		void SendToClient(const NetMessage& aMessage, ClientInfo& outClient);
		void SendGuaranteedToClient(const NetMessage& aMessage, ClientInfo& outClient);

		unsigned short GetMessageID();

		static std::string GetLogfileName();

	private:
		std::array<std::function<void(ClientInfo&, const NetMessage&)>, static_cast<size_t>(MessageType::Count)> myCallbacks;
		NetMessage myIncommingMessage;
		NetMessage myOutgoingMessage;
		WSADATA myWSA;
		std::mutex myMainMutex;
		std::mutex myConfirmationMutex;
		std::unique_ptr<MainLogger, std::function<void(MainLogger*)>> myLogger;

		std::unordered_map<std::string, ClientInfo> myClients;
		std::unordered_map<std::string, unsigned short> myClientIDs;
		std::unordered_map<std::string, std::vector<NetMessage>> myClientHistory;
		std::unordered_map < std::string, std::vector<ConfirmationData>> myWaitingConfirmations;
		std::vector<std::string> myRemovedClients;
		std::vector<NetMessage> myCachedMessages;

		sockaddr_in myClientInfo;
		sockaddr_in myServerInfo;
		SOCKET myServerSocket;
		PSTR myCurrentIP;
		std::thread* myThread;

		unsigned myIncommingDataAmount;
		unsigned myOutgoignDataAmount;
		unsigned mySentPacketsAmount;
		unsigned myLostPacketsAmount;

		float myResendTime;
		int mySocketSize;
		unsigned short myClientIDGenerator; // Very Hightech!
		unsigned short myMessageIDGenerator; // Very Hightech!

		uint8_t myMaxResendAttempts;
		bool myIsRunning;

		void ErrorShutDown();

		void HandlePacketLoss(float aPassedTime);

		void HandleConnect(ClientInfo& outClient, const std::string& anIdentifier);
		void HandleDisconnect(ClientInfo& aClient, const std::string& anIdentifier);
		void HandleConfirmation(const std::string& anIdentifier);
		void HandlePing(const std::string& anIdentifier);
		void HandleChat(ClientInfo& aClient, const std::string& anIdentifier);
		void HandleGameObjectMessage(const std::string& anIdentifier);

		void ActivateCallback(MessageType aType, ClientInfo& aClient, NetMessage& aMessage);

		void SetOutgoingMessageData(const std::string& aMessage);
		void ConfirmIncommingMessage(ClientInfo& aClient);

		std::string GetIdentifier(const char* anIP, unsigned short aPort);
		void RemoveClient(const std::string& anIdentifier);

		// Returns the error code
		int LogWSAError();
	};
}