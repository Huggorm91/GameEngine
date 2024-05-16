#include "Server.h"
#include "CrimsonUtilities/String/StringFunctions.h"
#include "NetworkShared/MessageFunctions.h"
#include <format>

#pragma comment (lib, "Ws2_32.lib")

namespace Network
{
	Server::Server() : myWSA(), myClientInfo(), myServerInfo(), myServerSocket(), myCurrentIP(nullptr), mySocketSize(sizeof(sockaddr_in)), myIsRunning(false)
	{}

	Server::~Server()
	{
		ShutDown();
		delete myCurrentIP;
	}

	void Server::Init()
	{
		myLogger = Logger::Create("Network Server");
		myLogger.SetPrintToFile(true, "Server Logs/" + Crimson::FileNameTimestamp() + ".txt");

		system("title Crimson Server");

		// Initialize winsock
		myLogger.Log("Initializing Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
		{
			myLogger.Err(std::format("Failed. Error Code: {}", WSAGetLastError()));
			ErrorShutDown();
		}

		// Create a socket
		myLogger.Log("Creating socket...");
		if ((myServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			myLogger.Err(std::format("Could not create socket: {}", WSAGetLastError()));
			ErrorShutDown();
		}

		// Prepare the sockaddr_in structure
		myServerInfo.sin_family = AF_INET;
		myServerInfo.sin_addr.s_addr = INADDR_ANY;
		myServerInfo.sin_port = htons(globalPort);

		// Bind socket
		myLogger.Log("Binding socket...");
		if (bind(myServerSocket, (sockaddr*)&myServerInfo, mySocketSize) == SOCKET_ERROR)
		{
			myLogger.Err(std::format("Bind failed with error code: {}", WSAGetLastError()));
			ErrorShutDown();
		}

		myIsRunning = true;
		myCurrentIP = new char[16];
		myLogger.Succ("Server initialized!");
	}

	void Server::Update()
	{
		fflush(stdout);
		ZeroMemory(&myClientInfo, sizeof(myClientInfo));
		ZeroMemory(&myMessage, sizeof(myMessage));
		ZeroMemory(myCurrentIP, 16);

		// try to receive some data, this is a blocking call
		if (recvfrom(myServerSocket, myMessage, sizeof(myMessage), 0, (sockaddr*)&myClientInfo, &mySocketSize) == SOCKET_ERROR)
		{
			auto error = WSAGetLastError();
			myLogger.Err(std::format("recvfrom() failed with error code: {}", error));
			if (error != 10054)
			{
				ErrorShutDown();
			}
			else
			{
				// Client has disconnected
				inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, myCurrentIP, 16);
				auto port = ntohs(myClientInfo.sin_port);
				myLogger.Warn(std::format("Client: {}:{} has been disconnected!", myCurrentIP, port));
				myClients.erase(GetIdentifier(myCurrentIP, port));
			}
			return;
		}

		// print details of the client/peer and the data received
		inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, myCurrentIP, 16);
		const unsigned short port = ntohs(myClientInfo.sin_port);

		const std::string identifier = GetIdentifier(myCurrentIP, port);

		ClientInfo client;
		client.socket = myClientInfo;
		client.ip = myCurrentIP;
		client.port = port;

		switch (myMessage.type)
		{
		case MessageType::Invalid:
			break;
		case MessageType::Connect:
		{
			HandleConnect(client, identifier);
			break;
		}
		case MessageType::Disconnect:
		{
			HandleDisconnect(client, identifier);
			break;
		}
		case MessageType::Chat:
		{
			HandleChat(client, identifier);
			break;
		}
		default:
			break;
		}

		SendMessageToClients(&client);

		for (auto& id : myRemovedClients)
		{
			myClients.erase(id);
		}
		myRemovedClients.clear();
	}

	bool Server::IsRunning() const
	{
		return myIsRunning;
	}

	void Server::ShutDown()
	{
		// Send message to clients to inform them the server has been turned off
		myMessage = CreateDisconnectMessage();
		SendMessageToClients(nullptr);

		// Cleanup
		closesocket(myServerSocket);
		WSACleanup();
	}

	void Server::ErrorShutDown()
	{
		if (myIsRunning)
		{
			closesocket(myServerSocket);
		}
		WSACleanup();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	void Server::HandleConnect(ClientInfo& outClient, const std::string& anIdentifier)
	{
		//outClient.username = myMessage.data;
		outClient.username = "Client" + std::to_string(outClient.port);

		myClients.emplace(anIdentifier, outClient);
		myLogger.Log(std::format("New connection from: {}\tUsername: {}", anIdentifier.c_str(), outClient.username.c_str()));

		SetMessageData(std::format("{} has joined the server.", outClient.username.c_str()));

		sendto(myServerSocket, CreateConfirmationMessage(), sizeof(NetMessage), 0, (sockaddr*)&outClient.socket, sizeof(sockaddr_in));
	}

	void Server::HandleDisconnect(const ClientInfo& aClient, const std::string& anIdentifier)
	{
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			SetMessageData(std::format("{} has disconnected.", iter->second.username.c_str()));
			myLogger.Log(std::format("Disconnect from: {}\tUsername: {}", anIdentifier.c_str(), iter->second.username.c_str()));
			myClients.erase(iter);
		}
		else
		{
			SetMessageData(std::format("UnknownUser{} has disconnected.", aClient.port));
			myLogger.Log(std::format("Unknown user disconnected: {}:{}", aClient.ip.c_str(), aClient.port));
		}
	}

	void Server::HandleConfirmation()
	{
	}

	void Server::HandlePing()
	{
	}

	void Server::HandleChat(const ClientInfo& aClient, const std::string& anIdentifier)
	{
		std::string message;
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			SetMessageData(std::format("{}: {}", iter->second.username.c_str(), myMessage.data));
			myLogger.Log(std::format("Message from: {}", myMessage.data));
		}
		else
		{
			SetMessageData(std::format("UnknownUser{}: {}", aClient.port, myMessage.data));
			myLogger.Log(std::format("Unknown user: {}:{}\nTried to send message: {}", aClient.ip.c_str(), aClient.port, myMessage.data));
		}
	}

	void Server::SetMessageData(const std::string& aMessage)
	{
		ZeroMemory(myMessage.data, globalBuffLength);
		myMessage.dataSize = static_cast<unsigned short>(aMessage.size() + 1);
		strcpy_s(myMessage.data, myMessage.dataSize, aMessage.c_str());
	}

	void Server::SendMessageToClients(ClientInfo* aCurrentClient)
	{
		for (auto& [id, entry] : myClients)
		{
			if (aCurrentClient && *aCurrentClient == entry)
			{
				continue;
			}

			if (sendto(myServerSocket, myMessage, sizeof(myMessage), 0, (sockaddr*)&entry.socket, sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				myLogger.Err(std::format("sendto() failed with error code: {}", WSAGetLastError()));
				++entry.failedMessageCount;
				if (entry.failedMessageCount > 5)
				{
					myLogger.Log(std::format("Client has failed to recieve too many messages. Client is now disconnected: {}\tUsername: {}", id.c_str(), entry.username.c_str()));
					myRemovedClients.emplace_back(id);
				}
			}
			else
			{
				entry.failedMessageCount = 0;
			}
		}
	}

	std::string Server::GetIdentifier(char* anIP, unsigned short aPort)
	{
		return std::string(anIP) + ':' + std::to_string(aPort);
	}

}