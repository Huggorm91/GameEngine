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
NetworkManager::NetworkManager() :
	myIncommingDataAmount(0u),
	myOutgoingDataAmount(0u),
	mySentPacketsAmount(0u),
	myLostPacketsAmount(0u),
	myLatency(0.f),
	myLatencyTimer(0.f),
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
		if (message.needReply)
		{
			myOutgoingDataAmount += message.dataSize;
			++mySentPacketsAmount;
		}

		switch (message.type)
		{
		case Network::MessageType::Invalid:
			break; // Ignore invalid messages
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
		case Network::MessageType::Ping:
		{
			myLatency = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - myPingTime).count();
			break;
		}
		case Network::MessageType::Confirmation:
		{
			for (auto iter = myWaitingConfirmations.begin(); iter != myWaitingConfirmations.end(); iter++)
			{
				if (*iter == message)
				{
					const bool shouldLookForCreate = iter->message.type == Network::MessageType::DeleteGameObject;
					myWaitingConfirmations.erase(iter);
					if (shouldLookForCreate)
					{
						for (auto createIter = myWaitingConfirmations.begin(); createIter != myWaitingConfirmations.end(); createIter++)
						{
							if (createIter->message.type == Network::MessageType::CreateGameObject && Network::ExtractUUID(createIter->message) == Network::ExtractUUID(message))
							{
								myWaitingConfirmations.erase(createIter);
								break;
							}
						}
					}
					break;
				}
			}
			break;
		}
		case Network::MessageType::GameObjectMessage:
		case Network::MessageType::CreateGameObject:
		{
			if (myRemovedUUIDs.contains(Network::ExtractUUID(message)))
			{
				continue;
			}
			myMessages.emplace_back(message);
			break;
		}
		case Network::MessageType::DeleteGameObject:
		{
			// TODO: Potential issue with memory if myRemovedUUIDs is not emptied every now and then
			myRemovedUUIDs.emplace(Network::ExtractUUID(message));
			myMessages.emplace_back(message);
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

	myLatencyTimer += Crimson::Time::GetDeltaTime();
	if (myLatencyTimer >= Network::globalPingFrequency)
	{
		myPingTime = std::chrono::high_resolution_clock::now();
		myClient->SendNetMessage(Network::CreatePingMessage());
		myLatencyTimer = 0.f;
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

bool NetworkManager::SendNetMessage(const Network::NetMessage& aMessage)
{
	assert(myClient && "Not initialized!");
	if (myClient->SendNetMessage(aMessage))
	{
		myOutgoingDataAmount += aMessage.dataSize;
		++mySentPacketsAmount;
		return true;
	}
	return false;
}

bool NetworkManager::SendGuaranteedNetMessage(const Network::NetMessage& aMessage, bool aShouldLimitRetries)
{
	const_cast<bool&>(aMessage.needReply) = true;
	if (SendNetMessage(aMessage))
	{
		myWaitingConfirmations.emplace_back(Network::ConfirmationData(aMessage, aShouldLimitRetries));
		return true;
	}
	return false;
}

bool NetworkManager::SendMultiNetMessage(const Network::NetMessage& aMessage)
{
	assert(myClient && "Not initialized!");
	if (myClient->SendMultipartMessage(aMessage))
	{
		myOutgoingDataAmount += aMessage.dataSize;
		++mySentPacketsAmount;
		return true;
	}
	return false;
}

bool NetworkManager::SendGuaranteedMultiNetMessage(const Network::NetMessage& aMessage, bool aShouldLimitRetries)
{
	const_cast<bool&>(aMessage.needReply) = true;
	if (SendMultiNetMessage(aMessage))
	{
		myWaitingConfirmations.emplace_back(Network::ConfirmationData(aMessage, aShouldLimitRetries));
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
	constexpr rsize_t messageSize = vectorSize + vectorSize + sizeof(double);

	Network::GameObjectMessage message;
	message.id = anID;
	message.action = Network::ObjectAction::Move;
	message.size = messageSize;

	// TODO: Rework timestamp to use a timepoint relative to servertime. 
	// Current idea: Server saves time when started and sends this timepoint to all clients that connects.
	// Should probably cache the current timepoint in Update
	double timestamp = Crimson::Time::GetTotalTime();
	constexpr int timestampOffset = vectorSize + vectorSize;
	memcpy_s(message.data, dataSize, &aTransform.GetPosition(), vectorSize);
	memcpy_s(message.data + vectorSize, dataSize - vectorSize, &aTransform.GetRotationRadian(), vectorSize);
	memcpy_s(message.data + timestampOffset, dataSize - timestampOffset, &timestamp, sizeof(double));

	SendNetMessage(Network::CreateGameObjectMessage(message));
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
	if (myIncommingDataAmount != 0 || myOutgoingDataAmount != 0)
	{
		std::string text = std::format("Network Statistics\nLatency: {}ms\nIncomming data: {} bytes\nOutgoing data : {} bytes\nPacketloss: {}/{}", myLatency, myIncommingDataAmount, myOutgoingDataAmount, myLostPacketsAmount, mySentPacketsAmount);
		myIncommingDataAmount = 0;
		myOutgoingDataAmount = 0;
		mySentPacketsAmount = 0;
		myLostPacketsAmount = 0;
		return text;
	}
	return "";
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
			if (data.shouldLimitRetries && data.amountSent >= myMaxResendAttempts)
			{
				++myLostPacketsAmount;
				iter = myWaitingConfirmations.erase(iter);
				continue;
			}

			myClient->SendNetMessage(data.message);
			data.timeSinceLastSend = 0.f;
			++data.amountSent;
			++myLostPacketsAmount;
		}
		++iter;
	}
}
