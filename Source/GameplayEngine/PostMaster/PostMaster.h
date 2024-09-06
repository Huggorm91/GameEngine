#pragma once
#include "Observer.h"
#include "Message.h"
#include <vector>
#include <unordered_map>
#include <shared_mutex>

namespace Crimson
{
	class PostMaster
	{
	public:
		PostMaster() = default;
		~PostMaster() = default;

		void Subscribe(Observer* anObserver, const eMessageType aType);

		void ClearSubscribers();
		void UnsubscribeFromAllMessages(Observer* anObserver);
		void UnsubscribeFromMessage(const eMessageType& aMessageType, Observer* anObserver);

		void AddMessage(const Message& aMessage);
		void SendInstantMessage(const Message& aMessage);

		void SendSavedMessages();

	private:
		std::shared_mutex myMutex;
		std::mutex mySecondaryMutex;
		std::vector<Message> myMessages;
		std::vector<Message> mySecondaryMessages;
		std::vector<std::pair<eMessageType, Observer*>> myAddList;
		std::vector<std::pair<eMessageType, Observer*>> myDeleteList;
		std::unordered_multimap<eMessageType, Observer*> myObservers;

		void SendMessageToSubscribers(const Message& aMessage) const;
		void RemoveSubscriber(const eMessageType& aMessageType, Observer* anObserver);
	};
}