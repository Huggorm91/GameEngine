#include "Client.h"
#include <winsock2.h>
#include <format>
#include "CrimsonUtilities/String/StringFunctions.h"
#include "CrimsonUtilities/Time/Timer.h"
#include "NetworkShared/MessageFunctions.h"

#pragma comment (lib, "Ws2_32.lib")

namespace Network
{
	Client::Client() : myWSA(), myServer(), mySocket(), myThread(nullptr), myFailedMessageCount(0u), myIsRunning(false),myIsConnected(false), myIsInitialized(false), myServerDisconnected(false)
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

		// Initialise winsock
		myLogger.Log("Initialising Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
		{
			myLogger.Warn(std::format("Failed. Error Code: {}", WSAGetLastError()));
			return;
		}

		// Create UDP socket
		myLogger.Log("Creating Socket...");
		if ((mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
		{
			myLogger.Warn(std::format("socket() failed with error code: {}", WSAGetLastError()));
			return;
		}

		u_long ne = TRUE;
		ioctlsocket(mySocket, FIONBIO, &ne);
		myIsInitialized = true;
		myLogger.Succ("Network client initialized!");
		myIsRunning = true;

		Connect();
	}

	bool Client::Connect()
	{
		myLogger.Log("Attempting to connect to server...");
		std::string IP = "127.0.0.1";
		unsigned short port = 27015;

		// Setup address structure
		memset((char*)&myServer, 0, sizeof(myServer));
		myServer.sin_family = AF_INET;
		myServer.sin_port = htons(port);
		if (!inet_pton(myServer.sin_family, IP.c_str(), &myServer.sin_addr.S_un.S_addr))
		{
			myLogger.Warn(std::format("Invalid IPv4 Address with error code: {}", WSAGetLastError()));
			return false;
		}

		// Create connect message
		std::string userName = "Default";

		// Send Connect message to Server
		if (sendto(mySocket, CreateConnectMessage(true, userName), sizeof(NetMessage), 0, (sockaddr*)&myServer, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			myLogger.Warn(std::format("Connect: sendto() failed with error code: {}", WSAGetLastError()));
			return false;
		}

		// Wait for reply from server
		NetMessage answer;
		int slen = sizeof(sockaddr_in);
		auto timer = Crimson::Timer::StartStopwatch();
		while (Crimson::Timer::StopStopwatch(timer) < 3.0)
		{
			const auto result = recvfrom(mySocket, answer, sizeof(answer), 0, (sockaddr*)&myServer, &slen);
			if (result != SOCKET_ERROR)
			{
				if (answer.type == MessageType::Confirmation)
				{
					myIsConnected = true;
					break;
				}
			}
		}

		if (myIsConnected)
		{
			myServerDisconnected = false;
			myLogger.Succ("Connected to Server.");
		}
		else
		{
			myLogger.Warn("Connection timed out. Could not connect to server.");
			return false;
		}

		if (myThread == nullptr)
		{
			myThread = new std::thread([this]() { this->Recieve(); });
		}
		return true;
	}

	void Client::Disconnect()
	{
		if (!myIsConnected)
		{
			return;
		}

		sendto(mySocket, CreateDisconnectMessage(), sizeof(NetMessage), 0, (sockaddr*)&myServer, sizeof(sockaddr_in));
		myIsConnected = false;
	}

	bool Client::SendNetMessage(const NetMessage& aMessage)
	{
		if (!myIsConnected)
		{
			return false;
		}

		if (myServerDisconnected)
		{
			myIsConnected = false;
			myLogger.Warn("Connection to server has been lost!");
			return false;
		}

		if (sendto(mySocket, aMessage, sizeof(aMessage), 0, (sockaddr*)&myServer, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			myLogger.Warn(std::format("SendNetMessage: sendto() failed with error code: {}", WSAGetLastError()));
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
		int error = 0;
		while (myIsRunning)
		{
			// Try to receive some data
			if (recvfrom(mySocket, answer, sizeof(answer), 0, (sockaddr*)&myServer, &slen) != SOCKET_ERROR)
			{
				std::unique_lock lock(myMutex);
				myMessages.emplace_back(answer);
			}
			else
			{
				error = WSAGetLastError();
				if (error == 10054) // Server has disconnected
				{
					myServerDisconnected = true;
				}				
			}
		}
	}

	bool Client::IsConnected() const
	{
		return myIsConnected;
	}
}