#define NOMINMAX
#include "GameplayEngine.pch.h"
#include "NetworkManager.h"
#include "NetworkClient/Client.h"
#include "GameObjectMessage.h"
#include "NetworkShared/MessageFunctions.h"
#include "NetworkShared/ConfirmationData.h"
#include "CrimsonUtilities/Math/Transform.h"
#include "CrimsonUtilities/Time/Time.h"
#include <assert.h>

// Needs to not be inline to avoid including Client.h in header
NetworkManager::NetworkManager():
	myIncommingDataAmount(0u),
	myOutgoignDataAmount(0u),
	mySentPacketsAmount(0u),
	myLostPacketsAmount(0u),
	myResendTime(1.f / 5.f),
	myMaxResendAttempts(3u)
{}

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
		myIncommingDataAmount += message.dataSize;
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
		case Network::MessageType::Confirmation:
		{
			for (auto iter = myWaitingConfirmations.begin(); iter != myWaitingConfirmations.end();)
			{
				if (*iter == message)
				{
					myWaitingConfirmations.erase(iter);
					break;
				}
			}
			break;
		}
		default:
		{
			myMessages.emplace_back(message);
			break;
		}
		}
	}
	HandlePacketLoss();
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

bool NetworkManager::SendNetMessage(const Network::NetMessage& aMessage)
{
	assert(myClient && "Not initialized!");
	if (myClient->SendNetMessage(aMessage))
	{
		myOutgoignDataAmount += aMessage.dataSize;
		++mySentPacketsAmount;
		return true;
	}
	return false;
}

bool NetworkManager::SendGuaranteedNetMessage(const Network::NetMessage& aMessage)
{
	const_cast<bool&>(aMessage.needReply) = true;
	if (SendNetMessage(aMessage))
	{
		myWaitingConfirmations.emplace_back(Network::ConfirmationData{ aMessage });
		return true;
	}
	return false;
}

bool NetworkManager::SendMultiNetMessage(const Network::NetMessage& aMessage)
{
	assert(myClient && "Not initialized!");
	if (myClient->SendMultipartMessage(aMessage))
	{
		myOutgoignDataAmount += aMessage.dataSize;
		++mySentPacketsAmount;
		return true;
	}
	return false;
}

bool NetworkManager::SendGuaranteedMultiNetMessage(const Network::NetMessage& aMessage)
{
	const_cast<bool&>(aMessage.needReply) = true;
	if (SendMultiNetMessage(aMessage))
	{
		myWaitingConfirmations.emplace_back(Network::ConfirmationData{ aMessage });
		return true;
	}
	return false;
}

void NetworkManager::SetTimeBetweenResend(float aTimeInSeconds)
{
	myResendTime = aTimeInSeconds;
}

void NetworkManager::SetMaximumResendAttempts(uint8_t anAmount)
{
	myMaxResendAttempts = anAmount;
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
	memcpy_s(message.data + vectorSize, dataSize - vectorSize, &aTransform.GetRotationRadian(), vectorSize);

	// TODO: Revert this to normal messages
	SendGuaranteedNetMessage(Network::CreateGameObjectMessage(message));
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
		SendGuaranteedNetMessage(Network::CreateCreateGameObjectMessage(anObject.GetUUID(), data));		
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
				memcpy_s(partialData.data(), bufferSize, pointer, bufferSize);
				pointer = pointer + bufferSize;
				dataLeft -= bufferSize;
			}

			Network::NetMessage message = Network::CreateCreateGameObjectMessage(anObject.GetUUID(), partialData);
			message.messageID = id;
			message.packetIndex = i;
			message.totalPackets = totalPackets;
			SendGuaranteedMultiNetMessage(message);
		}
	}
}

void NetworkManager::SendDeleteGameObject(const UUIDv4::UUID& anID)
{
	SendGuaranteedNetMessage(Network::CreateDeleteGameObjectMessage(anID));
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

std::string NetworkManager::GetStatisticsString()
{
	std::string text = std::format("Network Statistics\nIncomming data: {} bytes\nOutgoing data : {} bytes\nPacketloss: {}/{}", myIncommingDataAmount, myOutgoignDataAmount, myLostPacketsAmount, mySentPacketsAmount);
	myIncommingDataAmount = 0;
	myOutgoignDataAmount = 0;
	mySentPacketsAmount = 0;
	myLostPacketsAmount = 0;
	return text;
}

GameObject NetworkManager::ExtractCreatedGameObject(const Network::NetMessage& aMessage)
{	
	GameObject result(Network::ExtractUUID(aMessage));
	std::string stringData(aMessage.data + sizeof(UUIDv4::UUID), aMessage.dataSize - sizeof(UUIDv4::UUID));
	std::stringstream data(stringData);
	result.Deserialize(data);
	return result;
}

GameObject NetworkManager::ExtractCreatedGameObject(const std::vector<Network::NetMessage*>& aMessageList)
{
	GameObject result(Network::ExtractUUID(*aMessageList.front()));
	std::stringstream data;
	for (auto& message : aMessageList)
	{
		std::string stringData(message->data + sizeof(UUIDv4::UUID), message->dataSize - sizeof(UUIDv4::UUID));
		data << stringData;
	}

	result.Deserialize(data);
	return result;
}

void NetworkManager::HandlePacketLoss()
{
	for (auto iter = myWaitingConfirmations.begin(); iter != myWaitingConfirmations.end();)
	{
		auto& data = *iter;
		data.timeSinceLastSend += Crimson::Time::GetDeltaTime();
		if (data.timeSinceLastSend >= myResendTime)
		{
			if (data.amountSent >= myMaxResendAttempts)
			{
				++myLostPacketsAmount;
				iter = myWaitingConfirmations.erase(iter);
				continue;
			}

			myClient->SendNetMessage(data.message);
			data.timeSinceLastSend = 0.f;
			++data.amountSent;
		}
		++iter;
	}
}
