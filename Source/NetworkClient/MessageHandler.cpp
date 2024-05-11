#include "MessageHandler.h"
#include <assert.h>
#include "Client.h"

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
		case MessageType::Connect:
		case MessageType::Disconnect:
		case MessageType::Message:
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

void Network::MessageHandler::SendChatMessage(const std::string& aMessage)
{
	if (aMessage.empty())
	{
		return;
	}
	assert(myClient && "Not initialized!");

	NetMessage message;
	message.type = MessageType::Message;
	message.dataSize = static_cast<unsigned short>(aMessage.size() + 1);
	strcpy_s(message.data, message.dataSize, aMessage.c_str());

	myClient->SendNetMessage(message);
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
