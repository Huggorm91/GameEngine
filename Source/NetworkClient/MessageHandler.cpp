#include "MessageHandler.h"
#include <assert.h>
#include "Client.h"
#include "NetworkShared/MessageFunctions.h"
#include "CrimsonUtilities/Math/Transform.h"

Network::MessageHandler::MessageHandler() : myClient(nullptr)
{
}

Network::MessageHandler::~MessageHandler()
{
	delete myClient;
}

void Network::MessageHandler::Init()
{
	if (myClient == nullptr)
	{
		myClient = new Network::Client();
		myClient->Init();
		return;
	}
}

void Network::MessageHandler::Update()
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

void Network::MessageHandler::Connect()
{
	assert(myClient && "Not initialized!");
	myClient->Connect();
}

bool Network::MessageHandler::IsConnected() const
{
	assert(myClient && "Not initialized!");
	return myClient->IsConnected();
}

void Network::MessageHandler::SendNetMessage(const NetMessage& aMessage) const
{
	assert(myClient && "Not initialized!");
	myClient->SendNetMessage(aMessage);
}

void Network::MessageHandler::SendTransformChanged(const Transform& aTransform, unsigned anID)
{
	assert(myClient && "Not initialized!");
	myClient->SendNetMessage(CreateMoveGameObjectMessage(anID, aTransform.GetPosition(), aTransform.GetRotationRadian()));
}

void Network::MessageHandler::SendChatMessage(const std::string& aMessage)
{
	if (aMessage.empty())
	{
		return;
	}
	assert(myClient && "Not initialized!");

	myClient->SendNetMessage(CreateChatMessage(aMessage));
	myChatHistory.emplace_back(GetSelfHeader() + aMessage);
}

constexpr std::string Network::MessageHandler::GetSelfHeader() const
{
	return "Me:";
}

const std::vector<std::string>& Network::MessageHandler::GetChatHistory() const
{
	return myChatHistory;
}
