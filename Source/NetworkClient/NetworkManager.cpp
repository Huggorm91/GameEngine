#define NOMINMAX
#include "Client.h"
#include "NetworkManager.h"
#include <assert.h>
#include "NetworkShared/MessageFunctions.h"
#include "CrimsonUtilities/Math/Transform.h"

// Needs to not be inline to avoid including Client.h in header
Network::NetworkManager::NetworkManager()
{
}

// Needs to not be inline to avoid including Client.h in header
Network::NetworkManager::~NetworkManager()
{}

void Network::NetworkManager::Init()
{
	if (myClient == nullptr)
	{
		myClient = std::make_unique<Client>();
		myClient->Init();
		return;
	}
}

void Network::NetworkManager::Update()
{
	assert(myClient && "Not initialized!");
	auto messages = myClient->Flush();
	for (auto& message : messages)
	{
		switch (message.type)
		{
		case MessageType::Disconnect:
		{
			if (message.dataSize == 0)
			{
				myClient->myIsConnected = false;
				myClient->myLogger.Warn("Server has shut down.");
				myMessages.emplace_back(message);
			}
			else
			{
				myChatHistory.emplace_back(message.data);
			}
			break;
		}
		case MessageType::Connect:
		case MessageType::Chat:
		{
			myChatHistory.emplace_back(message.data);
			break;
		}
		default:
		{
			myMessages.emplace_back(message);
			break;
		}
		}		
	}
}

void Network::NetworkManager::Connect()
{
	assert(myClient && "Not initialized!");
	myClient->Connect();
}

bool Network::NetworkManager::IsConnected() const
{
	assert(myClient && "Not initialized!");
	return myClient->IsConnected();
}

void Network::NetworkManager::SendNetMessage(const NetMessage& aMessage) const
{
	assert(myClient && "Not initialized!");
	myClient->SendNetMessage(aMessage);
}

void Network::NetworkManager::SendTransformChanged(const Transform& aTransform, const UUIDv4::UUID& anID)
{
	assert(myClient && "Not initialized!");
	myClient->SendNetMessage(CreateMoveGameObjectMessage(anID, aTransform.GetPosition(), aTransform.GetRotationRadian()));
}

void Network::NetworkManager::SendChatMessage(const std::string& aMessage)
{
	if (aMessage.empty())
	{
		return;
	}
	assert(myClient && "Not initialized!");

	myClient->SendNetMessage(CreateChatMessage(aMessage));
	myChatHistory.emplace_back(GetSelfHeader() + aMessage);
}

constexpr std::string Network::NetworkManager::GetSelfHeader() const
{
	return "Me:";
}

const std::vector<std::string>& Network::NetworkManager::GetChatHistory() const
{
	return myChatHistory;
}

std::vector<Network::NetMessage>& Network::NetworkManager::GetMessages()
{
	return myMessages;
}
