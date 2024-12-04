#define NOMINMAX
#include "Server.h"
#include "CrimsonUtilities/String/StringFunctions.h"
#include "NetworkShared/MessageFunctions.h"
#include "NetworkShared/ConfirmationData.h"
#include <format>

#pragma comment (lib, "Ws2_32.lib")

namespace Network
{
	Server::Server() :
		myWSA(),
		myLogger(),
		myClientInfo(),
		myServerInfo(),
		myServerSocket(),
		myCurrentIP(nullptr),
		myThread(nullptr),
		myIncommingDataTotal(0u),
		myOutgoingDataTotal(0u),
		mySentPacketsTotal(0u),
		myLostPacketsTotal(0u),
		myResendTime(1.f / 5.f),
		mySocketSize(sizeof(sockaddr_in)),
		myClientIDGenerator(0u),
		myMessageIDGenerator(1u),
		myMaxResendAttempts(3u),
		myIsRunning(false)
	{}

	Server::~Server()
	{
		ShutDown();
		delete myCurrentIP;
	}

	void Server::Init(MainLogger* aLogger)
	{
		system("title Crimson Server");

		if (aLogger)
		{
			myLogger = std::unique_ptr<MainLogger, std::function<void(MainLogger*)>>(aLogger, [](MainLogger*) {});
		}
		else
		{
			myLogger = std::unique_ptr<MainLogger, std::function<void(MainLogger*)>>(new MainLogger(), [](MainLogger* p) { delete p; });
		}

		// Initialize winsock
		myLogger->Log("Initializing Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
		{
			myLogger->Err("Failed to initialize!");
			LogWSAError();
			ErrorShutDown();
		}

		// Create a socket
		myLogger->Log("Creating socket...");
		if ((myServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
		{
			myLogger->Err("Could not create socket!");
			LogWSAError();
			ErrorShutDown();
		}

		// Make socket non-blocking
		u_long ne = TRUE;
		if (ioctlsocket(myServerSocket, FIONBIO, &ne) == SOCKET_ERROR)
		{
			myLogger->Err("Could not make socket non-blocking!");
			LogWSAError();
			ErrorShutDown();
		}

		// Prepare the sockaddr_in structure
		myServerInfo.sin_family = AF_INET;
		myServerInfo.sin_addr.s_addr = INADDR_ANY;
		myServerInfo.sin_port = htons(globalPort);

		// Bind socket
		myLogger->Log("Binding socket...");
		if (bind(myServerSocket, (sockaddr*)&myServerInfo, mySocketSize) == SOCKET_ERROR)
		{
			myLogger->Err("Bind failed!");
			LogWSAError();
			ErrorShutDown();
		}

		myIsRunning = true;
		myCurrentIP = new char[16];
		myLogger->Succ("Server initialized!");

		if (myThread == nullptr)
		{
			myThread = new std::thread([this]() {
				while (myIsRunning)
				{
					this->Update();
				}
				});
		}
	}

	void Server::Update()
	{
		ZeroMemory(&myClientInfo, sizeof(myClientInfo));
		ZeroMemory(&myIncommingMessage, sizeof(myIncommingMessage));
		ZeroMemory(myCurrentIP, 16);

		// Try to receive some data
		if (recvfrom(myServerSocket, myIncommingMessage, sizeof(NetMessage), 0, (sockaddr*)&myClientInfo, &mySocketSize) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// No data to retrieve in socket
				return;
			}
			else if (WSAGetLastError() == WSAECONNRESET)
			{
				// Client has disconnected
				inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, myCurrentIP, 16);
				auto port = ntohs(myClientInfo.sin_port);
				myLogger->Warn(std::format("Client: {}:{} has been disconnected!", myCurrentIP, port));
				const auto& identifier = GetIdentifier(myCurrentIP, port);
				myClients.erase(identifier);
				myClientIDs.erase(identifier);
			}
			else
			{
				myLogger->Err("recvfrom() failed!");
				LogWSAError();
				ErrorShutDown();
			}
			return;
		}

		// Save details of the client
		inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, myCurrentIP, 16);
		const unsigned short port = ntohs(myClientInfo.sin_port);

		const std::string identifier = GetIdentifier(myCurrentIP, port);

		ClientInfo client;
		client.socket = myClientInfo;
		client.ip = myCurrentIP;
		client.port = port;

		ZeroMemory(&myOutgoingMessage, sizeof(myOutgoingMessage));
		{
			std::unique_lock lock(myMainMutex);
			myCachedMessages.emplace_back(myIncommingMessage);
			myIncommingDataTotal += myIncommingMessage.dataSize;

			myClientStatistics[identifier].incommingData += myIncommingMessage.dataSize;
		}

		bool needsNewMessageID = true;

		switch (myIncommingMessage.type)
		{
		case MessageType::Invalid:
			ActivateCallback(myIncommingMessage.type, client, myIncommingMessage);
			return; // Do not send these out to the clients
		case MessageType::Connect:
		{
			needsNewMessageID = HandleConnect(client, identifier);
			break;
		}
		case MessageType::Disconnect:
		{
			needsNewMessageID = HandleDisconnect(client, identifier);
			break;
		}
		case Network::MessageType::Confirmation:
		{
			HandleConfirmation(identifier);
			ActivateCallback(myIncommingMessage.type, client, myIncommingMessage);
			return; // These messages will not be sent to other clients
		}
		case MessageType::Chat:
		{
			needsNewMessageID = HandleChat(client, identifier);
			break;
		}
		case Network::MessageType::Ping:
		{
			HandlePing(identifier);
			ActivateCallback(myIncommingMessage.type, client, myIncommingMessage);
			return; // Should not send to all
		}
		case Network::MessageType::GameObjectMessage:
		{
			needsNewMessageID = HandleGameObjectMessage(identifier);
			break;
		}
		case Network::MessageType::CreateGameObject:
		{
			needsNewMessageID = HandleGameObjectMessage(identifier);
			break;
		}
		case Network::MessageType::DeleteGameObject:
		{
			needsNewMessageID = HandleGameObjectMessage(identifier);
			break;
		}
		default:
			return; // Unknown Messagetype, do not send to clients
		}

		ActivateCallback(myIncommingMessage.type, client, myIncommingMessage);

		if (needsNewMessageID)
		{
			SendMessageToClients(myOutgoingMessage, &client);
		}
		else
		{
			SendMultiMessageToClients(myOutgoingMessage, &client);
		}

		for (auto& id : myRemovedClients)
		{
			RemoveClient(id);
		}
		myRemovedClients.clear();
	}

	std::vector<NetMessage> Server::Flush(float aTimeSinceLastFlushInSeconds)
	{
		HandlePacketLoss(aTimeSinceLastFlushInSeconds);
		std::unique_lock lock(myMainMutex);
		std::vector<NetMessage> copy = std::move(myCachedMessages);
		myCachedMessages.clear();
		return copy;
	}

	void Server::ReportStatistics()
	{
		std::unique_lock lock(myMainMutex);
		if (myIncommingDataTotal != 0 || myOutgoingDataTotal != 0)
		{
			std::string text =	"------------------------------------\n"
								"Network Statistics\n";			

			for (auto& [id, stats] : myClientStatistics)
			{
				text += std::format("\n{}:\n"
					"Incomming data: {} bytes\n"
					"Outgoing data : {} bytes\n"
					"Packetloss: {}/{}\n",
					id,
					stats.incommingData,
					stats.outgoingData,
					stats.lostPackets, stats.sentPackets);

				stats.Reset();
			}

			text += std::format("\nTotal:\n"
								"Incomming data: {} bytes\n"
								"Outgoing data : {} bytes\n"
								"Packetloss: {}/{}\n"
								"------------------------------------", 
								myIncommingDataTotal, 
								myOutgoingDataTotal, 
								myLostPacketsTotal, mySentPacketsTotal);

			myIncommingDataTotal = 0;
			myOutgoingDataTotal = 0;
			mySentPacketsTotal = 0;
			myLostPacketsTotal = 0;

			// Send directly to cout to not flood log-file with statistics
			std::cout << text << std::endl;
		}		
	}

	void Server::SetTimeBetweenResend(float aTimeInSeconds)
	{
		myResendTime = aTimeInSeconds;
	}

	void Server::SetMaximumResendAttempts(uint8_t anAmount)
	{
		myMaxResendAttempts = anAmount;
	}

	bool Server::IsRunning() const
	{
		return myIsRunning;
	}

	void Server::ShutDown()
	{
		if (myThread)
		{
			myIsRunning = false;
			myThread->join();
		}

		// Send message to clients to inform them the server has been turned off
		myOutgoingMessage = CreateDisconnectMessage();
		SendMessageToClients(myOutgoingMessage);

		// Cleanup
		closesocket(myServerSocket);
		WSACleanup();

		myLogger->PrintHistoryToFile(GetLogfileName());
	}

	void Server::SetIncommingMessageCallback(MessageType aType, const std::function<void(ClientInfo&, const NetMessage&)>& aFunction)
	{
		myCallbacks[static_cast<size_t>(aType)] = aFunction;
	}

	void Server::SendToClient(const NetMessage& aMessage, ClientInfo& outClient)
	{
		const_cast<unsigned short&>(aMessage.messageID) = GetMessageID();
		SendToClientInternal(aMessage, outClient);
	}

	void Server::SendGuaranteedToClient(const NetMessage& aMessage, ClientInfo& outClient, bool aShouldLimitRetries)
	{
		const_cast<unsigned short&>(aMessage.messageID) = GetMessageID();
		SendGuaranteedToClientInternal(aMessage, outClient, aShouldLimitRetries);
	}

	void Server::SendMessageToClients(const NetMessage& aMessage, ClientInfo* aClientToAvoid)
	{
		const_cast<unsigned short&>(aMessage.messageID) = GetMessageID();
		if (aClientToAvoid)
		{
			const auto& clientToAvoid = *aClientToAvoid;
			for (auto& [id, entry] : myClients)
			{
				if (clientToAvoid == entry)
				{
					continue;
				}
				SendToClientInternal(aMessage, entry);
			}
		}
		else
		{
			for (auto& [id, entry] : myClients)
			{
				SendToClientInternal(aMessage, entry);
			}
		}
	}

	void Server::SendGuaranteedMessageToClients(const NetMessage& aMessage, ClientInfo* aClientToAvoid, bool aShouldLimitRetries)
	{
		const_cast<unsigned short&>(aMessage.messageID) = GetMessageID();
		if (aClientToAvoid)
		{
			const auto& clientToAvoid = *aClientToAvoid;
			for (auto& [id, entry] : myClients)
			{
				if (clientToAvoid == entry)
				{
					continue;
				}
				SendGuaranteedToClientInternal(aMessage, entry, aShouldLimitRetries);
			}
		}
		else
		{
			for (auto& [id, entry] : myClients)
			{
				SendGuaranteedToClientInternal(aMessage, entry, aShouldLimitRetries);
			}
		}
	}

	void Server::SendMultiMessageToClient(const NetMessage& aMessage, ClientInfo& outClient, bool aShouldLimitRetries)
	{
		if (aMessage.needReply)
		{
			SendGuaranteedToClientInternal(aMessage, outClient, aShouldLimitRetries);
		}
		else
		{
			SendToClientInternal(aMessage, outClient);
		}
	}

	void Server::SendMultiMessageToClients(const NetMessage& aMessage, ClientInfo* aClientToAvoid, bool aShouldLimitRetries)
	{
		if (aClientToAvoid)
		{
			const auto& clientToAvoid = *aClientToAvoid;
			if (aMessage.needReply)
			{
				for (auto& [id, entry] : myClients)
				{
					if (clientToAvoid == entry)
					{
						continue;
					}
					SendGuaranteedToClientInternal(aMessage, entry, aShouldLimitRetries);
				}
			}
			else
			{
				for (auto& [id, entry] : myClients)
				{
					if (clientToAvoid == entry)
					{
						continue;
					}
					SendToClientInternal(aMessage, entry);
				}
			}			
		}
		else
		{
			if (aMessage.needReply)
			{
				for (auto& [id, entry] : myClients)
				{
					SendGuaranteedToClientInternal(aMessage, entry, aShouldLimitRetries);
				}
			}
			else
			{
				for (auto& [id, entry] : myClients)
				{
					SendToClientInternal(aMessage, entry);
				}
			}			
		}
	}

	unsigned short Server::GetMessageID()
	{
		return myMessageIDGenerator++;
	}

	std::string Server::GetLogfileName()
	{
		return "Server Logs/" + Crimson::FileNameTimestamp() + ".txt";
	}

	void Server::ErrorShutDown()
	{
		bool wasRunning = myIsRunning;
		if (myThread)
		{
			myIsRunning = false;
			myThread->join();
		}
		if (wasRunning)
		{
			closesocket(myServerSocket);
		}
		WSACleanup();

		myLogger->PrintHistoryToFile(GetLogfileName());

		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	void Server::SendToClientInternal(const NetMessage& aMessage, ClientInfo& outClient)
	{
		if (sendto(myServerSocket, aMessage, aMessage.GetCurrentSize(), 0, (sockaddr*)&outClient.socket, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			myLogger->Err("sendto() failed!");
			LogWSAError();
			++outClient.failedMessageCount;
			if (outClient.failedMessageCount > 5)
			{
				const auto& identifier = GetIdentifier(outClient.ip.c_str(), outClient.port);
				myLogger->Log(std::format("Client has failed to recieve too many messages. Client is now disconnected: {}\tUsername: {}", identifier, outClient.username));
				myRemovedClients.emplace_back(identifier);
			}
		}
		else
		{
			outClient.failedMessageCount = 0;
			std::unique_lock lock(myMainMutex);
			myOutgoingDataTotal += aMessage.dataSize;
			++mySentPacketsTotal;

			const auto& identifier = GetIdentifier(outClient.ip.c_str(), outClient.port);
			auto& stats = myClientStatistics[identifier];
			stats.outgoingData += aMessage.dataSize;
			++stats.sentPackets;
		}
	}

	void Server::SendGuaranteedToClientInternal(const NetMessage& aMessage, ClientInfo& outClient, bool aShouldLimitRetries)
	{
		const_cast<bool&>(aMessage.needReply) = true;
		SendToClientInternal(aMessage, outClient);
		const auto& identifier = GetIdentifier(outClient.ip.c_str(), outClient.port);
		std::unique_lock lock(myConfirmationMutex);
		myWaitingConfirmations[identifier].emplace_back(ConfirmationData(aMessage, aShouldLimitRetries));
	}

	void Server::HandlePacketLoss(float aTimeSinceLastCallInSeconds)
	{
		std::unique_lock lock(myConfirmationMutex);
		for (auto& [id, dataList] : myWaitingConfirmations)
		{
			for (auto iter = dataList.begin(); iter != dataList.end();)
			{
				auto& data = *iter;
				data.timeSinceLastSend += aTimeSinceLastCallInSeconds;
				if (data.timeSinceLastSend >= myResendTime)
				{
					if (data.shouldLimitRetries && data.amountSent >= myMaxResendAttempts)
					{
						++myLostPacketsTotal;
						if (auto statIter = myClientStatistics.find(id); statIter != myClientStatistics.end())
						{
							++(statIter->second.lostPackets);
						}
						iter = dataList.erase(iter);
						continue;
					}

					if (myClients.contains(id))
					{
						// Do not assign a new MessageID
						SendToClientInternal(data.message, myClients.at(id));
						data.timeSinceLastSend = 0.f;
						++data.amountSent;
						++myLostPacketsTotal;
						++(myClientStatistics[id].lostPackets);
					}
					else
					{
						// Client has disconnected
						iter = dataList.erase(iter);
						continue;
					}
				}
				++iter;
			}
		}
	}

	bool Server::HandleConnect(ClientInfo& outClient, const std::string& anIdentifier)
	{
		outClient.username = "Client" + std::to_string(outClient.port);

		myLogger->Log(std::format("New connection from: {}\tUsername: {}", anIdentifier, outClient.username));
		SetOutgoingMessageData(std::format("{} has joined the server.", outClient.username));

		myClients.emplace(anIdentifier, outClient);
		myClientStatistics.emplace(anIdentifier, ClientStatistics());
		myClientIDs.emplace(anIdentifier, ++myClientIDGenerator);

		auto message = CreateConfirmationMessage();
		message.dataSize = sizeof(myClientIDGenerator);
		memcpy_s(message.data, globalBuffLength, &myClientIDGenerator, sizeof(myClientIDGenerator));
		sendto(myServerSocket, message, message.GetCurrentSize(), 0, (sockaddr*)&outClient.socket, sizeof(sockaddr_in));
		{
			std::unique_lock lock(myMainMutex);
			myOutgoingDataTotal += message.dataSize;
			++mySentPacketsTotal;

			auto& stats = myClientStatistics[anIdentifier];
			stats.outgoingData += message.dataSize;
			++stats.sentPackets;
		}

		myClientHistory.emplace(anIdentifier, std::vector<NetMessage>()).first->second.emplace_back(myIncommingMessage);
		return true;
	}

	bool Server::HandleDisconnect(ClientInfo& aClient, const std::string& anIdentifier)
	{
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			SetOutgoingMessageData(std::format("{} has disconnected.", iter->second.username));
			myLogger->Log(std::format("Disconnect from: {}\tUsername: {}", anIdentifier, iter->second.username));
			RemoveClient(anIdentifier);
		}
		else
		{
			SetOutgoingMessageData(std::format("UnknownUser{} has disconnected.", aClient.port));
			myLogger->Log(std::format("Unknown user disconnected: {}:{}", aClient.ip, aClient.port));
		}

		return true;
	}

	void Server::HandleConfirmation(const std::string& anIdentifier)
	{
		std::unique_lock lock(myConfirmationMutex);
		auto& dataList = myWaitingConfirmations[anIdentifier];
		for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
		{
			if (*iter == myIncommingMessage)
			{
				//const bool isDestroyMessage = iter->message.type == Network::MessageType::DeleteGameObject;

				// TODO: Fix this check, for unknown reasons the check of UUIDs crashes in Release
				//if (isDestroyMessage && Network::ExtractUUID(iter->message) != Network::ExtractUUID(myIncommingMessage))
				//{
				//	// UUID has been corrupted
				//	return;
				//}
				dataList.erase(iter);

				// TODO: Fix this check, for unknown reasons the check of UUIDs crashes in Release
				//if (isDestroyMessage)
				//{
				//	// Remove any CreateObject messages with the same UUID
				//	for (auto createIter = dataList.begin(); createIter != dataList.end(); createIter++)
				//	{
				//		if (createIter->message.type == Network::MessageType::CreateGameObject && Network::ExtractUUID(createIter->message) == Network::ExtractUUID(myIncommingMessage))
				//		{
				//			dataList.erase(createIter);
				//			break;
				//		}
				//	}
				//}
				return;
			}
		}
	}

	void Server::HandlePing(const std::string& anIdentifier)
	{
		if (myIncommingMessage.needReply)
		{
			if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
			{
				SendToClient(myIncommingMessage, iter->second);
			}
			else
			{
				myLogger->Log(std::format("Ping from Unknown user: {}", anIdentifier));
			}
		}
		else
		{
			// This would be a return call from someone the server has pinged
		}


	}

	bool Server::HandleChat(ClientInfo& aClient, const std::string& anIdentifier)
	{
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			SetOutgoingMessageData(std::format("{}: {}", iter->second.username, myIncommingMessage.data));
			myLogger->Log(std::format("{} sent message: {}", iter->second.username, myIncommingMessage.data));
			ConfirmIncommingMessage(aClient);
		}
		else
		{
			SetOutgoingMessageData(std::format("UnknownUser{}: {}", aClient.port, myIncommingMessage.data));
			myLogger->Log(std::format("Unknown user: {}:{}\tSent message: {}", aClient.ip, aClient.port, myIncommingMessage.data));
		}

		return true;
	}

	bool Server::HandleGameObjectMessage(const std::string& anIdentifier)
	{
		myOutgoingMessage = myIncommingMessage;
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			// TODO: Make sure only the latest message of each UUID and type is saved
			myClientHistory[anIdentifier].emplace_back(myIncommingMessage);
			ConfirmIncommingMessage(iter->second);
		}

		return false;
	}

	void Server::ActivateCallback(MessageType aType, ClientInfo& aClient, NetMessage& aMessage)
	{
		if (myCallbacks[static_cast<size_t>(aType)])
		{
			myCallbacks[static_cast<size_t>(aType)](aClient, aMessage);
		}
	}

	void Server::SetOutgoingMessageData(const std::string& aMessage)
	{
		myOutgoingMessage.dataSize = static_cast<unsigned short>(aMessage.size() + 1);
		strcpy_s(myOutgoingMessage.data, myOutgoingMessage.dataSize, aMessage.c_str());
	}

	void Server::ConfirmIncommingMessage(ClientInfo& aClient)
	{
		if (myIncommingMessage.needReply)
		{
			auto message = myIncommingMessage;
			message.needReply = false;
			message.type = MessageType::Confirmation;
			SendToClient(message, aClient);
		}
	}

	std::string Server::GetIdentifier(const char* anIP, unsigned short aPort)
	{
		return std::string(anIP) + ':' + std::to_string(aPort);
	}

	void Server::RemoveClient(const std::string& anIdentifier)
	{
		myClientIDs.erase(anIdentifier);
		myClientHistory.erase(anIdentifier);
		myWaitingConfirmations.erase(anIdentifier);
		myClientStatistics.erase(anIdentifier);
		myClients.erase(anIdentifier);
	}

	int Server::LogWSAError()
	{
		int error = WSAGetLastError();

		char buffer[256]('\0');

		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,		// flags
			NULL,															// lpsource
			error,															// message id
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),						// languageid
			buffer,															// output buffer
			sizeof(buffer),													// size of msgbuf, bytes
			NULL															// va_list of arguments
		);

		if (buffer)
		{
			myLogger->Err(std::format("WSA error code {}: {}", error, buffer));
		}
		else
		{
			myLogger->Err(std::format("Unknown WSA error code: {}", error));
		}

		return error;
	}
}