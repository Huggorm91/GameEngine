#include "Client.h"
#include <winsock2.h>
#include <iostream>
#include "Shared/Globals.h"
#include "Shared/NetMessage.h"
#include <string>

#pragma comment (lib, "Ws2_32.lib")

Client::Client() : myThread(nullptr), mySocket(), myIsRunning(false), myIsInitialized(false), myIsConnected(false), myServer()
{}

Client::~Client()
{
	if (myThread)
	{
		myIsRunning = false;
		myThread->join();
	}

	if (myIsConnected)
	{
		NetMessage message;
		message.type = MessageType::Disconnect;
		sendto(mySocket, message, sizeof(message), 0, (sockaddr*)&myServer, sizeof(sockaddr_in));

	}

	if (myIsInitialized)
	{
		closesocket(mySocket);
		WSACleanup();
	}
}

void Client::Init()
{
	myIsRunning = true;
	// initialise winsock
	WSADATA ws;
	printf("Initialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
	{
		printf("Failed. Error Code: %d", WSAGetLastError());
	}
	printf("Initialised.\n");

	// create socket

	if ((mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
	{
		printf("socket() failed with error code: %d", WSAGetLastError());
	}
	myIsInitialized = true;

	u_long ne = TRUE;
	ioctlsocket(mySocket, FIONBIO, &ne);

	if (Connect())
	{
		myIsConnected = true;
		myThread = new std::thread([this]() { this->Recieve(); });
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
			printf("sendto() failed with error code: %d", WSAGetLastError());
		}
	}
}

bool Client::Connect()
{
	printf("Enter IP: (Default: 127.0.0.1)\n");
	std::string IPInput;
	std::getline(std::cin, IPInput);
	if (IPInput.empty())
	{
		IPInput = "127.0.0.1";
	}

	printf("Enter Port: (Default: 27015)\n");
	std::string port;
	std::getline(std::cin, port);
	if (port.empty())
	{
		port = "27015";
	}

	// setup address structure
	memset((char*)&myServer, 0, sizeof(myServer));
	myServer.sin_family = AF_INET;
	myServer.sin_port = htons(stoi(port));
	if (!inet_pton(myServer.sin_family, IPInput.c_str(), &myServer.sin_addr.S_un.S_addr))
	{
		printf("invalid IPv4 Address with error code: %d", WSAGetLastError());
		return false;
	}

	printf("Enter Username: \n");
	std::string nameInput;
	std::cin >> nameInput;

	NetMessage message;
	message.type = MessageType::Connect;
	message.dataSize = static_cast<unsigned short>(nameInput.size() + 1);
	strcpy_s(message.data, message.dataSize, nameInput.c_str());

	if (sendto(mySocket, message, sizeof(message), 0, (sockaddr*)&myServer, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code: %d", WSAGetLastError());
		return false;
	}
	return true;
}

void Client::SendNetMessage(const NetMessage& aMessage)
{
	if (myIsConnected)
	{
		if (sendto(mySocket, aMessage, sizeof(aMessage), 0, (sockaddr*)&myServer, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code: %d", WSAGetLastError());
		}
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
