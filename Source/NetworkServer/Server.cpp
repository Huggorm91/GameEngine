#include "Server.h"
#include "CrimsonUtilities/String/StringFunctions.h"
#include <format>

#pragma comment (lib, "Ws2_32.lib")

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
		myLogger.Err(std::format("Failed. Error Code: %d\n", WSAGetLastError()));
		ErrorShutDown();
	}

	// Create a socket
	myLogger.Log("Creating socket...");
	if ((myServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		myLogger.Err(std::format("Could not create socket: %d\n", WSAGetLastError()));
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
		myLogger.Err(std::format("Bind failed with error code: %d\n", WSAGetLastError()));
		ErrorShutDown();
	}

	myIsRunning = true;
	myLogger.Succ("Server initialized!");
}

void Server::Update()
{
	fflush(stdout);

	// try to receive some data, this is a blocking call
	if (recvfrom(myServerSocket, myMessage, sizeof(myMessage), 0, (sockaddr*)&myClientInfo, &mySocketSize) == SOCKET_ERROR)
	{
		myLogger.Err(std::format("recvfrom() failed with error code: %d\n", WSAGetLastError()));
		ErrorShutDown();
	}

	// print details of the client/peer and the data received
	PSTR clientIP = new char[16];
	inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, clientIP, 16);
	auto port = ntohs(myClientInfo.sin_port);

	std::string identifier = std::string(clientIP) + ':' + std::to_string(port);

	Client client;
	client.socket = myClientInfo;
	client.ip = clientIP;
	client.port = ntohs(myClientInfo.sin_port);

	std::string userName = client.username;

	switch (myMessage.type)
	{
	case MessageType::Invalid:
		break;
	case MessageType::Connect:
	{
		client.username = myMessage.data;
		myClients.emplace(identifier, client);
		myLogger.Log(std::format("New connection from: %s\tUsername: %s\n", identifier.c_str(), client.username.c_str()));
		sendto(myServerSocket, "Successfully joined server.", static_cast<int>(strlen("Successfully joined server.")), 0, (sockaddr*)&client.socket, sizeof(sockaddr_in));
		break;
	}
	case MessageType::Disconnect:
	{
		if (auto iter = myClients.find(identifier); iter != myClients.end())
		{
			userName = iter->second.username;
			myClients.erase(iter);
			myLogger.Log(std::format("Disconnect from: %s\tUsername: %s\n", identifier.c_str(), userName.c_str()));
		}
		else
		{
			myLogger.Log(std::format("Unknown user disconnected: %s:%d\n", client.ip.c_str(), client.port));
		}
		break;
	}
	case MessageType::Message:
	{
		if (auto iter = myClients.find(identifier); iter != myClients.end())
		{
			userName = iter->second.username;
			myLogger.Log(std::format("%s: %s\n", userName.c_str(), myMessage.data));
		}
		else
		{
			myLogger.Log(std::format("Unknown user: %s:%d\nTried to send message: %s\n", client.ip.c_str(), client.port, myMessage.data));
		}
		break;
	}
	default:
		break;
	}

	for (auto& [iter, name] : myClients)
	{
		if (client == name)
		{
			continue;
		}

		if (sendto(myServerSocket, myMessage, sizeof(myMessage), 0, (sockaddr*)&name.socket, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			myLogger.Err(std::format("sendto() failed with error code: %d\n", WSAGetLastError()));
			ErrorShutDown();
		}
	}
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
}

void Server::ErrorShutDown()
{
	WSACleanup();
	exit(EXIT_FAILURE);
}
