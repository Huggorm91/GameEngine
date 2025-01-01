#pragma once
#include <vector>
#include <string>
#include <chrono>
#include "NetworkShared/NetMessage.h"

class Transform;
class GameObject;

namespace Network
{
	class Client;
	struct ConfirmationData;
}

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void Init();
	void Update();

	void Connect();
	bool IsConnected() const;

	bool SendNetMessage(const Network::NetMessage& aMessage);
	bool SendGuaranteedNetMessage(const Network::NetMessage& aMessage, bool aShouldLimitRetries = true);

	void SetTimeBetweenResend(float aTimeInSeconds);
	void SetMaximumResendAttempts(uint8_t anAmount);

	void SendTransformChanged(const Transform& aTransform, float aTimeUntilNextSync, const UUIDv4::UUID& anID);

	void SendCreateGameObject(const GameObject& anObject);
	void SendDeleteGameObject(const UUIDv4::UUID& anID);

	void SendSetActiveMessage(bool aState, const UUIDv4::UUID& anID);

	void SendChatMessage(const std::string& aMessage);
	constexpr std::string GetSelfHeader() const;

	const std::vector<std::string>& GetChatHistory() const;
	std::vector<Network::NetMessage>& GetMessages();
	void ClearMessages();

	std::string GetStatisticsString();

	static GameObject ExtractCreatedGameObject(const Network::NetMessage& aMessage);
	static GameObject ExtractCreatedGameObject(const std::vector<Network::NetMessage*>& aMessageList);

private:
	std::unordered_set<UUIDv4::UUID> myRemovedUUIDs;
	std::vector<std::string> myChatHistory;
	std::vector<Network::NetMessage> myMessages;
	std::vector<Network::ConfirmationData> myWaitingConfirmations;
	std::unique_ptr<Network::Client> myClient;

	std::chrono::high_resolution_clock::time_point myPingTime;

	unsigned myIncommingDataAmount;
	unsigned myOutgoingDataAmount;
	unsigned mySentPacketsAmount;
	unsigned myLostPacketsAmount;

	float myLatency;
	float myLatencyTimer;

	float myResendTime;
	uint8_t myMaxResendAttempts;

	// These are private for now since MessageID is not handled correctly in them
	bool SendMultiNetMessage(const Network::NetMessage& aMessage);
	bool SendGuaranteedMultiNetMessage(const Network::NetMessage& aMessage, bool aShouldLimitRetries = true);

	void HandlePacketLoss();
};