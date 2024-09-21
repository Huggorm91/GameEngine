#define NOMINMAX
#include "Client.h"
#include <winsock2.h>
#include <format>
#include "CrimsonUtilities/String/StringFunctions.h"
#include "CrimsonUtilities/Time/Time.h"
#include "NetworkShared/MessageFunctions.h"

#pragma comment (lib, "Ws2_32.lib")

namespace Network
{
	Client::Client() :
		myWSA(),
		myLogger("Network Logs/" + Crimson::FileNameTimestamp() + ".txt"),
		myServer(),
		mySocket(),
		myThread(nullptr),
		myFailedMessageCount(0u),
		mySenderID(0u),
		myIDGenerator(0u),
		myHasError(false),
		myIsRunning(false),
		myIsConnected(false),
		myIsInitialized(false),
		myServerDisconnected(false)
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
		// Initialise winsock
		myLogger.Log("Initialising Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
		{
			myLogger.Warn(std::format("WSAStartup failed with error Code: {}", WSAGetLastError()));
			return;
		}

		// Create UDP socket
		myLogger.Log("Creating Socket...");
		if ((mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
		{
			myLogger.Warn(std::format("socket() failed with error code: {}", WSAGetLastError()));
			return;
		}

		// Make socket non-blocking
		u_long ne = TRUE;
		if (ioctlsocket(mySocket, FIONBIO, &ne) == SOCKET_ERROR)
		{
			myLogger.Warn(std::format("ioctlsocket() failed with error code: {}", WSAGetLastError()));
			return;
		}
		myIsInitialized = true;
		myLogger.Succ("Network client initialized!");
		myIsRunning = true;

		Connect();
	}

	bool Client::Connect()
	{
		myLogger.Log("Attempting to connect to server...");
		constexpr const char* IP = "127.0.0.1";
		constexpr unsigned short port = 27015;
		constexpr double timeoutValue = 1.;

		// Setup address structure
		memset((char*)&myServer, 0, sizeof(myServer));
		myServer.sin_family = AF_INET;
		myServer.sin_port = htons(port);
		if (!inet_pton(myServer.sin_family, IP, &myServer.sin_addr.S_un.S_addr))
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
		auto timer = Crimson::Time::StartTimer();
		while (Crimson::Time::StopTimer(timer) < timeoutValue)
		{
			const auto result = recvfrom(mySocket, answer, sizeof(answer), 0, (sockaddr*)&myServer, &slen);
			if (result != SOCKET_ERROR)
			{
				if (answer.type == MessageType::Confirmation)
				{
					mySenderID = reinterpret_cast<unsigned short&>(answer.data);
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
		if (myIsConnected)
		{
			sendto(mySocket, CreateDisconnectMessage(), sizeof(NetMessage), 0, (sockaddr*)&myServer, sizeof(sockaddr_in));
			myIsConnected = false;
		}
	}

	void Client::HandleMultiMessages()
	{
		std::sort(myMultipartMessages.begin(), myMultipartMessages.end(), MultiMessageSort);
		unsigned short currentSender = 0;
		unsigned short currentMessage = 0;
		unsigned short previousIndex = 0;

		// TODO: Add timer that checks for time since last message in a chain was recieved

		std::vector<NetMessage> messagesToMove;
		for (auto iter = myMultipartMessages.begin(); iter != myMultipartMessages.end();)
		{
			const auto& message = *iter;
			if (currentSender != message.senderID)
			{
				currentSender = message.senderID;
				currentMessage = message.messageID;
				previousIndex = message.packetIndex;
				iter++;
				continue;
			}
			if (currentMessage != message.messageID)
			{
				currentMessage = message.messageID;
				previousIndex = message.packetIndex;
				iter++;
				continue;
			}
			if (previousIndex != message.packetIndex -1)
			{
				// Missing a packet
				// TODO: Send request for replacement
				iter++;
				continue;
			}

			unsigned short totalIndex = message.totalPackets - 1;
			if (message.packetIndex == totalIndex)
			{
				messagesToMove.insert(messagesToMove.end(), std::make_move_iterator(iter - totalIndex) , std::make_move_iterator(iter + 1));
				iter = myMultipartMessages.erase(iter - totalIndex, iter + 1);
			}
			else
			{
				previousIndex = message.packetIndex;
				iter++;
			}
		}

		if (!messagesToMove.empty())
		{
			std::unique_lock lock(myMutex);
			myMessages.insert(myMessages.end(), std::make_move_iterator(messagesToMove.begin()), std::make_move_iterator(messagesToMove.end()));
		}
	}

	void Client::LogLastError()
	{
		if (myHasError)
		{
			myHasError = false;
			myLogger.Warn(myLastError);
		}
	}

	bool Client::CanSendMessage()
	{
		if (!myIsConnected)
		{
			return false;
		}

		LogLastError();

		if (myServerDisconnected)
		{
			myIsConnected = false;
			myLogger.Warn("Connection to server has been lost!");
			return false;
		}
		return true;
	}

	bool Client::SendNetMessageInternal(const NetMessage& aMessage)
	{
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

	bool Client::SendNetMessage(const NetMessage& aMessage)
	{
		if (!CanSendMessage())
		{
			return false;
		}

		const_cast<unsigned short&>(aMessage.senderID) = mySenderID;
		const_cast<unsigned short&>(aMessage.messageID) = myIDGenerator++;

		return SendNetMessageInternal(aMessage);
	}

	bool Client::SendMultipartMessage(const NetMessage& aMessage)
	{
		if (!CanSendMessage())
		{
			return false;
		}

		const_cast<unsigned short&>(aMessage.senderID) = mySenderID;

		return SendNetMessageInternal(aMessage);
	}

	unsigned short Client::GetMessageID()
	{
		return myIDGenerator++;
	}

	std::vector<NetMessage> Client::Flush()
	{
		LogLastError();
		std::unique_lock lock(myMutex);
		std::vector<NetMessage> copy;
		copy.swap(myMessages);
		return copy;
	}

	std::string Client::GetStatisticsString()
	{
		std::unique_lock lock(myMutex);
		std::string text = std::format("Network Statistics\nIncomming data: {} bytes\nOutgoing data : {} bytes\nPacketloss: {}/{}", myIncommingDataAmount, myOutgoignDataAmount, myLostPacketsAmount, mySentPacketsAmount);
		myIncommingDataAmount = 0;
		myOutgoignDataAmount = 0;
		mySentPacketsAmount = 0;
		myLostPacketsAmount = 0;
		return text;
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
				if (answer.totalPackets == 1)
				{
					std::unique_lock lock(myMutex);
					myMessages.emplace_back(answer);
				}
				else
				{
					myMultipartMessages.emplace_back(answer);
					HandleMultiMessages();
				}
			}
			else
			{
				error = WSAGetLastError();
				if (error == WSAEWOULDBLOCK) // No data to retrieve in socket
				{
					continue;
				}
				else if (error == WSAECONNRESET) // Server has disconnected
				{
					myServerDisconnected = true;
				}

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
					myLastError = buffer;
				}
				else
				{
					myLastError = "Unknown error: " + std::to_string(error);
				}
				myHasError = true;
			}
		}
	}

	bool Client::IsConnected() const
	{
		return myIsConnected;
	}
}