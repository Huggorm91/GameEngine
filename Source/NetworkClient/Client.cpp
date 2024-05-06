#include "Client.h"
#include <winsock2.h>
#include <iostream>
#include "Shared/Globals.h"
#include "Shared/NetMessage.h"
#include <string>
#include <format>
#include "CrimsonUtilities/String/StringFunctions.h"

#pragma comment (lib, "Ws2_32.lib")

Client::Client() : myThread(nullptr), mySocket(), myIsRunning(false), myIsInitialized(false), myIsConnected(false), myServer(), myFailedMessageCount(0u)
{}

Client::~Client()
{
	if (myThread)
	{
		myIsRunning = false;
		myThread->join();
	}

	Disconnect();

	if (myIsInitialized)
	{
		closesocket(mySocket);
		WSACleanup();
	}
}

void Client::Init()
{
	myLogger = Logger::Create("Network Client");
	myLogger.SetPrintToFile(true, "Network Logs/" + Crimson::FileNameTimestamp() + ".txt");
	myIsRunning = true;

	// Initialise winsock
	WSADATA ws;
	myLogger.Log("Initialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
	{
		myLogger.Warn(std::format("Failed. Error Code: %d", WSAGetLastError()));
	}

	// Create UDP socket
	myLogger.Log("Creating Socket...");
	if ((mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		myLogger.Warn(std::format("socket() failed with error code: %d", WSAGetLastError()));
	}

	u_long ne = TRUE;
	ioctlsocket(mySocket, FIONBIO, &ne);
	myIsInitialized = true;
	myLogger.Succ("Initialised.");

	if (Connect())
	{
		myLogger.Succ("Connected to Server.");
	}
	else
	{
		myLogger.Warn("Failed to connect to Server.");
	}
}

void Client::Update()
{
	std::string input;
	std::getline(std::cin, input);
	if (!input.empty())
	{
		NetMessage message;
		message.type = MessageType::Message;
		message.dataSize = static_cast<unsigned short>(input.size() + 1);
		strcpy_s(message.data, message.dataSize, input.c_str());

		if (sendto(mySocket, message, sizeof(message), 0, (sockaddr*)&myServer, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			myLogger.Log(std::format("sendto() failed with error code: %d", WSAGetLastError()));
		}
	}
}

bool Client::Connect()
{
	std::string IP = "127.0.0.1";
	unsigned port = 27015;

	// Setup address structure
	memset((char*)&myServer, 0, sizeof(myServer));
	myServer.sin_family = AF_INET;
	myServer.sin_port = htons(port);
	if (!inet_pton(myServer.sin_family, IP.c_str(), &myServer.sin_addr.S_un.S_addr))
	{
		myLogger.Warn(std::format("invalid IPv4 Address with error code: %d", WSAGetLastError()));
		return false;
	}

	// Create connect message
	std::string userName = "Default";

	NetMessage message;
	message.type = MessageType::Connect;
	message.dataSize = static_cast<unsigned short>(userName.size() + 1);
	strcpy_s(message.data, message.dataSize, userName.c_str());

	// Send Connect message to Server
	if (sendto(mySocket, message, sizeof(message), 0, (sockaddr*)&myServer, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		myLogger.Warn(std::format("Connect: sendto() failed with error code: %d", WSAGetLastError()));
		return false;
	}

	if (myThread == nullptr)
	{
		myThread = new std::thread([this]() { this->Recieve(); });
	}
	myIsConnected = true;
	return true;
}

void Client::Disconnect()
{
	if (!myIsConnected)
	{
		return;
	}

	NetMessage message;
	message.type = MessageType::Disconnect;
	sendto(mySocket, message, sizeof(message), 0, (sockaddr*)&myServer, sizeof(sockaddr_in));
	myIsConnected = false;
}

bool Client::SendNetMessage(const NetMessage& aMessage)
{
	if (!myIsConnected)
	{
		return;
	}

	if (sendto(mySocket, aMessage, sizeof(aMessage), 0, (sockaddr*)&myServer, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		myLogger.Warn(std::format("SendNetMessage: sendto() failed with error code: %d", WSAGetLastError()));
		++myFailedMessageCount;

		if (myFailedMessageCount > 5u)
		{
			Disconnect();
			myIsConnected = false;
			myFailedMessageCount = 0u;
			myLogger.Warn("Client has failed to send too many messages. Client is now disconnected!");
		}
		return false;
	}
	else
	{
		myFailedMessageCount = 0u;
		return true;
	}
}

std::vector<NetMessage> Client::Flush()
{
	std::unique_lock lock(myMutex);
	std::vector<NetMessage> copy;
	copy.swap(myMessages);
	return copy;
}

void Client::Recieve()
{
	NetMessage answer;
	int slen = sizeof(sockaddr_in);
	while (myIsRunning)
	{
		// Try to receive some data
		if (recvfrom(mySocket, answer, globalBuffLength, 0, (sockaddr*)&myServer, &slen) != SOCKET_ERROR)
		{
			std::unique_lock lock(myMutex);
			myMessages.emplace_back(answer);
		}
	}
}

bool Client::IsConnected() const
{
	return myIsConnected;
}
