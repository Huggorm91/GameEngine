#include "Server.h"
#include "CrimsonUtilities/String/StringFunctions.h"
#include <format>

#pragma comment (lib, "Ws2_32.lib")

namespace Network
{
	void Server::Init()
	{
		myLogger = Logger::Create("Network Server");
		myLogger.SetPrintToFile(true, "Server Logs/" + Crimson::FileNameTimestamp() + ".txt");
		myIsRunning = true;
		mySocketSize = sizeof(sockaddr_in);

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
		myLogger.Succ("Server initialized!");
	}

	void Server::Update()
	{
		fflush(stdout);
		ZeroMemory(&myClientInfo, sizeof(myClientInfo));
		ZeroMemory(&myMessage, sizeof(myMessage));

		// try to receive some data, this is a blocking call
		if (recvfrom(myServerSocket, myMessage, sizeof(myMessage), 0, (sockaddr*)&myClientInfo, &mySocketSize) == SOCKET_ERROR)
		{
			auto error = WSAGetLastError();
			myLogger.Err(std::format("recvfrom() failed with error code: {}", error));
			if (error != 10054) // Client has disconnected
			{
				ErrorShutDown();
			}
			else
			{
				PSTR clientIP = new char[16];
				inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, clientIP, 16);
				auto port = ntohs(myClientInfo.sin_port);
				myLogger.Warn(std::format("Client: {}:{} has been disconnected!", clientIP, port));
				std::string identifier = std::string(clientIP) + ':' + std::to_string(port);
				myClients.erase(identifier);
			}
			return;
		}

		// print details of the client/peer and the data received
		PSTR clientIP = new char[16];
		inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, clientIP, 16);
		auto port = ntohs(myClientInfo.sin_port);

		std::string identifier = std::string(clientIP) + ':' + std::to_string(port);

		ClientInfo client;
		client.socket = myClientInfo;
		client.ip = clientIP;
		client.port = port;

		std::string userName = client.username;

		switch (myMessage.type)
		{
		case MessageType::Invalid:
			break;
		case MessageType::Connect:
		{
			client.username = myMessage.data;
			myClients.emplace(identifier, client);
			myLogger.Log(std::format("New connection from: {}\tUsername: {}", identifier.c_str(), client.username.c_str()));
			NetMessage message;
			message.type = MessageType::Confirmation;
			sendto(myServerSocket, message, sizeof(myMessage), 0, (sockaddr*)&client.socket, sizeof(sockaddr_in));
			break;
		}
		case MessageType::Disconnect:
		{
			if (auto iter = myClients.find(identifier); iter != myClients.end())
			{
				userName = iter->second.username;
				myClients.erase(iter);
				myLogger.Log(std::format("Disconnect from: {}\tUsername: {}", identifier.c_str(), userName.c_str()));
			}
			else
			{
				myLogger.Log(std::format("Unknown user disconnected: {}:{}", client.ip.c_str(), client.port));
			}
			break;
		}
		case MessageType::Message:
		{
			if (auto iter = myClients.find(identifier); iter != myClients.end())
			{
				userName = iter->second.username;
				myLogger.Log(std::format("{}: {}", userName.c_str(), myMessage.data));
			}
			else
			{
				myLogger.Log(std::format("Unknown user: {}:{}\nTried to send message: {}", client.ip.c_str(), client.port, myMessage.data));
			}
			break;
		}
		default:
			break;
		}

		for (auto& [id, entry] : myClients)
		{
			if (client == entry)
			{
				continue;
			}

			if (sendto(myServerSocket, myMessage, sizeof(myMessage), 0, (sockaddr*)&entry.socket, sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				myLogger.Err(std::format("sendto() failed with error code: {}", WSAGetLastError()));
				++entry.failedMessageCount;
				if (entry.failedMessageCount > 5)
				{
					myLogger.Log(std::format("Client has failed to recieve too many messages. Client is now disconnected: {}\tUsername: {}", id.c_str(), userName.c_str()));
					myRemovedClients.emplace_back(id);
				}
			}
			else
			{
				entry.failedMessageCount = 0;
			}
		}

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
		// TODO:
		// Disconnect all clients
		closesocket(myServerSocket);
		WSACleanup();
		system("PAUSE");
	}

	void Server::ErrorShutDown()
	{
		WSACleanup();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
}