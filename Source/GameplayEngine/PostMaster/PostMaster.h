#pragma once
#include "Observer.h"
#include "Message.h"
#include <vector>
#include <unordered_map>
#include <shared_mutex>

class PostMaster
{
public:
	PostMaster() = default;
	~PostMaster() = default;

	void Subscribe(Observer* anObserver, const Crimson::eMessageType aType);

	void ClearSubscribers();
	void UnsubscribeFromAllMessages(Observer* anObserver);
	void UnsubscribeFromMessage(const Crimson::eMessageType& aMessageType, Observer* anObserver);

	void AddMessage(const Crimson::Message& aMessage);
	void SendInstantMessage(const Crimson::Message& aMessage);

	void SendSavedMessages();

private:
	std::shared_mutex myMutex;
	std::mutex mySecondaryMutex;
	std::vector<Crimson::Message> myMessages;
	std::vector<Crimson::Message> mySecondaryMessages;
	std::vector<std::pair<Crimson::eMessageType, Observer*>> myAddList;
	std::vector<std::pair<Crimson::eMessageType, Observer*>> myDeleteList;
	std::unordered_multimap<Crimson::eMessageType, Observer*> myObservers;

	void SendMessageToSubscribers(const Crimson::Message& aMessage) const;
	void RemoveSubscriber(const Crimson::eMessageType& aMessageType, Observer* anObserver);
};