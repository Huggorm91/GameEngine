#define NOMINMAX
#include "GameplayEngine.pch.h"
#include "NetworkManager.h"
#include "NetworkClient/Client.h"
#include "GameObjectMessage.h"
#include "NetworkShared/MessageFunctions.h"
#include "CrimsonUtilities/Math/Transform.h"
#include <assert.h>

// Needs to not be inline to avoid including Client.h in header
NetworkManager::NetworkManager()
{
}

// Needs to not be inline to avoid including Client.h in header
NetworkManager::~NetworkManager()
{}

void NetworkManager::Init()
{
	if (myClient == nullptr)
	{
		myClient = std::make_unique<Network::Client>();
		myClient->Init();
	}
}

void NetworkManager::Update()
{
	assert(myClient && "Not initialized!");
	auto messages = myClient->Flush();
	for (auto& message : messages)
	{
		switch (message.type)
		{
		case Network::MessageType::Disconnect:
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
		case Network::MessageType::Connect:
		case Network::MessageType::Chat:
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

void NetworkManager::Connect()
{
	assert(myClient && "Not initialized!");
	myClient->Connect();
}

bool NetworkManager::IsConnected() const
{
	assert(myClient && "Not initialized!");
	return myClient->IsConnected();
}

void NetworkManager::SendNetMessage(const Network::NetMessage& aMessage) const
{
	assert(myClient && "Not initialized!");
	myClient->SendNetMessage(aMessage);
}

void NetworkManager::SendTransformChanged(const Transform& aTransform, const UUIDv4::UUID& anID)
{
	assert(myClient && "Not initialized!");
	constexpr rsize_t dataSize = sizeof(Network::GameObjectMessage::data);
	constexpr rsize_t vectorSize = sizeof(Crimson::Vector3f);

	Network::GameObjectMessage message;
	message.id = anID;
	message.action = Network::ObjectAction::Move;
	message.size = vectorSize + vectorSize;

	memcpy_s(message.data, dataSize, &aTransform.GetPosition(), vectorSize);
	memcpy_s(message.data + vectorSize, dataSize, &aTransform.GetRotationRadian(), vectorSize);

	myClient->SendNetMessage(Network::CreateGameObjectMessage(message));
}

void NetworkManager::SendCreateGameObject(const GameObject& anObject)
{
	std::vector<uint8_t> data;
	{
		std::stringstream stream;
		anObject.Serialize(stream, true);
		data.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	}

	if (data.size() <= Network::GetMaximumCreateGameobjectDataSize())
	{
		myClient->SendNetMessage(Network::CreateCreateGameObjectMessage(anObject.GetUUID(), data));
	}
	else
	{
		constexpr unsigned short bufferSize = Network::GetMaximumCreateGameobjectDataSize();
		unsigned short totalPackets = static_cast<unsigned short>(data.size() / bufferSize);
		if (data.size() % bufferSize > 0u)
		{
			// Add one due to int rounding from previous division
			++totalPackets;
		}
		unsigned short id = myClient->GetMessageID();
		size_t dataLeft = data.size();
		uint8_t* pointer = data.data();
		for (unsigned short i = 0; i < totalPackets; i++)
		{
			std::vector<uint8_t> partialData;
			if (dataLeft < bufferSize)
			{
				partialData.resize(dataLeft);
				memcpy_s(partialData.data(), dataLeft, pointer, dataLeft);
			}
			else
			{
				partialData.resize(bufferSize);
				memcpy_s(partialData.data(), bufferSize, pointer, dataLeft);
				pointer = pointer + bufferSize;
				dataLeft -= bufferSize;
			}

			Network::NetMessage message = Network::CreateCreateGameObjectMessage(anObject.GetUUID(), partialData);
			message.messageID = id;
			message.packetIndex = i;
			message.totalPackets = totalPackets;
			myClient->SendMultipartMessage(message);
		}
	}
}

void NetworkManager::SendDeleteGameObject(const UUIDv4::UUID& anID)
{
	myClient->SendNetMessage(Network::CreateDeleteGameObjectMessage(anID));
}

void NetworkManager::SendChatMessage(const std::string& aMessage)
{
	if (aMessage.empty())
	{
		return;
	}
	assert(myClient && "Not initialized!");

	myClient->SendNetMessage(Network::CreateChatMessage(aMessage));
	myChatHistory.emplace_back(GetSelfHeader() + aMessage);
}

constexpr std::string NetworkManager::GetSelfHeader() const
{
	return "Me:";
}

const std::vector<std::string>& NetworkManager::GetChatHistory() const
{
	return myChatHistory;
}

std::vector<Network::NetMessage>& NetworkManager::GetMessages()
{
	return myMessages;
}

void NetworkManager::ClearMessages()
{
	myMessages.clear();
}

GameObject NetworkManager::ExtractCreatedGameObject(const Network::NetMessage& aMessage)
{	
	GameObject result(Network::ExtractUUID(aMessage));
	std::stringstream data(std::string(aMessage.data + sizeof(UUIDv4::UUID), aMessage.dataSize - sizeof(UUIDv4::UUID)));
	result.Deserialize(data);
	return result;
}
