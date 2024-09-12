#define NOMINMAX
#include "Server.h"
#include "CrimsonUtilities/String/StringFunctions.h"
#include "NetworkShared/MessageFunctions.h"
#include <format>

#pragma comment (lib, "Ws2_32.lib")

namespace Network
{
	static std::string GetLogfileName()
	{
		return "Server Logs/" + Crimson::FileNameTimestamp() + ".txt";
	}
	Server::Server() :
		myWSA(),
		myLogger(),
		myClientInfo(),
		myServerInfo(),
		myServerSocket(),
		myCurrentIP(nullptr),
		mySocketSize(sizeof(sockaddr_in)),
		myIDGenerator(0),
		myIsRunning(false)
	{}

	Server::~Server()
	{
		ShutDown();
		delete myCurrentIP;
	}

	void Server::Init()
	{
		system("title Crimson Server");

		// Initialize winsock
		myLogger.Log("Initializing Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
		{
			myLogger.Err("Failed to initialize!");
			LogWSAError();
			ErrorShutDown();
		}

		// Create a socket
		myLogger.Log("Creating socket...");
		if ((myServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			myLogger.Err("Could not create socket!");
			LogWSAError();
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
			myLogger.Err("Bind failed!");
			LogWSAError();
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
		if (recvfrom(myServerSocket, myMessage, sizeof(NetMessage), 0, (sockaddr*)&myClientInfo, &mySocketSize) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != 10054)
			{
				myLogger.Err("recvfrom() failed!");
				LogWSAError();
				ErrorShutDown();
			}
			else
			{
				// Client has disconnected
				inet_ntop(myServerInfo.sin_family, &myClientInfo.sin_addr, myCurrentIP, 16);
				auto port = ntohs(myClientInfo.sin_port);
				myLogger.Warn(std::format("Client: {}:{} has been disconnected!", myCurrentIP, port));
				const auto& identifier = GetIdentifier(myCurrentIP, port);
				myClients.erase(identifier);
				myClientIDs.erase(identifier);
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

		// TODO: Save all messages sent by clients in a searchable list, so that they can be resent in case of packet loss
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
		case Network::MessageType::Confirmation:
		{
			HandleConfirmation();
			break;
		}
		case Network::MessageType::ResendMessage:
		{
			break;
		}
		case MessageType::Chat:
		{
			HandleChat(client, identifier);
			break;
		}
		case Network::MessageType::Ping:
		{
			HandlePing(identifier);
			return; // Should not send to all
		}
		case Network::MessageType::GameObjectMessage:
		{
			HandleGameObjectMessage(client, identifier);
			break;
		}
		case Network::MessageType::CreateGameObject:
		{
			HandleGameObjectMessage(client, identifier);
			break;
		}
		case Network::MessageType::DeleteGameObject:
		{
			HandleGameObjectMessage(client, identifier);
			break;
		}
		default:
			break;
		}

		SendMessageToClients(&client);

		for (auto& id : myRemovedClients)
		{
			myClients.erase(id);
			myClientIDs.erase(identifier);
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

		myLogger.PrintHistoryToFile(GetLogfileName());
	}

	void Server::ErrorShutDown()
	{
		if (myIsRunning)
		{
			closesocket(myServerSocket);
		}
		WSACleanup();

		myLogger.PrintHistoryToFile(GetLogfileName());

		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	void Server::HandleConnect(ClientInfo& outClient, const std::string& anIdentifier)
	{
		outClient.username = "Client" + std::to_string(outClient.port);

		myLogger.Log(std::format("New connection from: {}\tUsername: {}", anIdentifier, outClient.username));
		SetMessageData(std::format("{} has joined the server.", outClient.username));

		myClients.emplace(anIdentifier, outClient);
		myClientIDs.emplace(anIdentifier, ++myIDGenerator);

		auto message = CreateConfirmationMessage();
		memcpy_s(message.data, globalBuffLength, &myIDGenerator, sizeof(unsigned short));
		sendto(myServerSocket, message, sizeof(NetMessage), 0, (sockaddr*)&outClient.socket, sizeof(sockaddr_in));

		for (auto& [id, history] : myClientHistory)
		{
			for (auto& oldMessage : history)
			{
				sendto(myServerSocket, oldMessage, sizeof(NetMessage), 0, (sockaddr*)&outClient.socket, sizeof(sockaddr_in));
			}
		}
		myClientHistory.emplace(anIdentifier, std::vector<NetMessage>()).first->second.emplace_back(myMessage);
	}

	void Server::HandleDisconnect(const ClientInfo& aClient, const std::string& anIdentifier)
	{
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			SetMessageData(std::format("{} has disconnected.", iter->second.username));
			myLogger.Log(std::format("Disconnect from: {}\tUsername: {}", anIdentifier, iter->second.username));
			myClientIDs.erase(iter->first);
			myClientHistory.erase(iter->first);
			myClients.erase(iter);
		}
		else
		{
			SetMessageData(std::format("UnknownUser{} has disconnected.", aClient.port));
			myLogger.Log(std::format("Unknown user disconnected: {}:{}", aClient.ip, aClient.port));
		}
	}

	void Server::HandleConfirmation()
	{
	}

	void Server::HandlePing(const std::string& anIdentifier)
	{
		if (myMessage.needReply)
		{
			if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
			{
				SendToClient(iter->second, anIdentifier);
				myLogger.Log(std::format("Ping from: {}\tUsername: {}", anIdentifier, iter->second.username));
			}
			else
			{
				myLogger.Log(std::format("Ping from Unknown user: {}", anIdentifier));
			}
		}
		else
		{
			// This would be a return call from someone the server has pinged
		}
	}

	void Server::HandleChat(const ClientInfo& aClient, const std::string& anIdentifier)
	{
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			SetMessageData(std::format("{}: {}", iter->second.username, myMessage.data));
			myLogger.Log(std::format("{} sent message: {}", iter->second.username, myMessage.data));
		}
		else
		{
			SetMessageData(std::format("UnknownUser{}: {}", aClient.port, myMessage.data));
			myLogger.Log(std::format("Unknown user: {}:{}\tSent message: {}", aClient.ip, aClient.port, myMessage.data));
		}
	}

	void Server::HandleGameObjectMessage(const ClientInfo& aClient, const std::string& anIdentifier)
	{
		if (auto iter = myClients.find(anIdentifier); iter != myClients.end())
		{
			myLogger.Log(std::format("{} sent {}byte data to Object with UUID: {}", iter->second.username, myMessage.dataSize, ExtractUUID(myMessage).str()));
			// TODO: Make sure only the latest message of each UUID and type is saved
			myClientHistory[anIdentifier].emplace_back(myMessage);
		}
		else
		{
			myLogger.Log(std::format("Unknown user: {}:{}\tSent {}byte data to Object with UUID: {}", aClient.ip, aClient.port, myMessage.dataSize, ExtractUUID(myMessage).str()));
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

			SendToClient(entry, id);
		}
	}

	void Server::SendToClient(ClientInfo& outClient, const std::string& anIdentifier)
	{
		if (sendto(myServerSocket, myMessage, sizeof(NetMessage), 0, (sockaddr*)&outClient.socket, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			myLogger.Err("sendto() failed!");
			LogWSAError();
			++outClient.failedMessageCount;
			if (outClient.failedMessageCount > 5)
			{
				myLogger.Log(std::format("Client has failed to recieve too many messages. Client is now disconnected: {}\tUsername: {}", anIdentifier, outClient.username));
				myRemovedClients.emplace_back(anIdentifier);
			}
		}
		else
		{
			outClient.failedMessageCount = 0;
		}
	}

	std::string Server::GetIdentifier(char* anIP, unsigned short aPort)
	{
		return std::string(anIP) + ':' + std::to_string(aPort);
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
			myLogger.Err(std::format("WSA error code {}: {}", error, buffer));
		}
		else
		{
			myLogger.Err(std::format("Unknown WSA error code: {}", error));
		}

		return error;
	}
}