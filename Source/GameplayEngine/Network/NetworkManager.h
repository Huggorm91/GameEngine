#pragma once
#include <vector>
#include <string>
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
	bool SendGuaranteedNetMessage(const Network::NetMessage& aMessage);

	void SetTimeBetweenResend(float aTimeInSeconds);
	void SetMaximumResendAttempts(uint8_t anAmount);

	void SendTransformChanged(const Transform& aTransform, const UUIDv4::UUID& anID);

	void SendCreateGameObject(const GameObject& anObject);
	void SendDeleteGameObject(const UUIDv4::UUID& anID);

	void SendChatMessage(const std::string& aMessage);
	constexpr std::string GetSelfHeader() const;

	const std::vector<std::string>& GetChatHistory() const;
	std::vector<Network::NetMessage>& GetMessages();
	void ClearMessages();

	std::string GetStatisticsString();

	static GameObject ExtractCreatedGameObject(const Network::NetMessage& aMessage);
	static GameObject ExtractCreatedGameObject(const std::vector<Network::NetMessage*>& aMessageList);

private:
	std::vector<std::string> myChatHistory;
	std::vector<Network::NetMessage> myMessages;
	std::vector<Network::ConfirmationData> myWaitingConfirmations;
	std::unique_ptr<Network::Client> myClient;

	unsigned myIncommingDataAmount;
	unsigned myOutgoignDataAmount;
	unsigned mySentPacketsAmount;
	unsigned myLostPacketsAmount;

	float myResendTime;
	uint8_t myMaxResendAttempts;

	// These are private for now since MessageID is not handled correctly in them
	bool SendMultiNetMessage(const Network::NetMessage& aMessage);
	bool SendGuaranteedMultiNetMessage(const Network::NetMessage& aMessage);

	void HandlePacketLoss();
};