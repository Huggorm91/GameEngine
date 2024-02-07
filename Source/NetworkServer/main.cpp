#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <unordered_map>
#include "Shared/NetMessage.h"

#pragma comment (lib, "Ws2_32.lib")

int main()
{
	system("title Crimson Server");

	sockaddr_in server, clientSocket;

	// initialise winsock
	WSADATA wsa;
	printf("Initialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code: %d\n", WSAGetLastError());
		exit(0);
	}
	printf("Initialised.\n");

	// create a socket
	SOCKET server_socket;
	if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket: %d\n", WSAGetLastError());
	}
	printf("Socket created.\n");

	// prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(globalPort);

	// bind
	if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code: %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done.\n");

	struct Client
	{
		sockaddr_in socket;
		std::string username;
		std::string ip;
		unsigned short port = 0;

		inline bool operator==(const Client& aClient)
		{
			return aClient.ip == ip && aClient.port == port;
		}
	};
	std::unordered_map<std::string, Client> clients;

	printf("Waiting for data...\n");
	int slen = sizeof(sockaddr_in);
	while (true)
	{
		fflush(stdout);
		NetMessage message;

		// try to receive some data, this is a blocking call
		if (recvfrom(server_socket, message, sizeof(message), 0, (sockaddr*)&clientSocket, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code: %d\n", WSAGetLastError());
			exit(0);
		}

		// print details of the client/peer and the data received
		PSTR clientIP = new char[16];
		inet_ntop(server.sin_family, &clientSocket.sin_addr, clientIP, 16);
		auto port = ntohs(clientSocket.sin_port);

		std::string identifier = std::string(clientIP) + ':' + std::to_string(port);

		Client client;
		client.socket = clientSocket;
		client.ip = clientIP;
		client.port = ntohs(clientSocket.sin_port);

		std::string userName = client.username;

		switch (message.type)
		{
		case MessageType::Invalid:
			break;
		case MessageType::Connect:
		{
			client.username = message.data;
			clients.emplace(identifier, client);
			printf("New connection from: %s\tUsername: %s\n", identifier.c_str(), client.username.c_str());
			sendto(server_socket, "Successfully joined server.", static_cast<int>(strlen("Successfully joined server.")), 0, (sockaddr*)&client.socket, sizeof(sockaddr_in));
			break;
		}
		case MessageType::Disconnect:
		{
			if (auto iter = clients.find(identifier); iter != clients.end())
			{
				userName = iter->second.username;
				clients.erase(iter);
				printf("Disconnect from: %s\tUsername: %s\n", identifier.c_str(), userName.c_str());
			}
			else
			{
				printf("Unknown user disconnected: %s:%d\n", client.ip.c_str(), client.port);
			}
			break;
		}
		case MessageType::Message:
		{
			if (auto iter = clients.find(identifier); iter != clients.end())
			{
				userName = iter->second.username;
				printf(std::string(userName + ": " + message.data).c_str());
				std::cout << std::endl;
			}
			else
			{
				printf("Unknown user: %s:%d\nTried to send message: %s\n", client.ip.c_str(), client.port, message.data);
			}
			break;
		}
		default:
			break;
		}

		for (auto& [iter, name] : clients)
		{
			if (client == name)
			{
				continue;
			}

			if (sendto(server_socket, message, sizeof(message), 0, (sockaddr*)&name.socket, sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				printf("sendto() failed with error code: %d\n", WSAGetLastError());
				return 3;
			}
		}
	}

	closesocket(server_socket);
	WSACleanup();

	return 0;
}