#pragma once
#include "Observer.h"
#include "Message.h"
#include <vector>
#include <unordered_map>

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
		void UnsubscribeFromMessage(const eMessageType& aMsgType, Observer* anObserver);

		void AddMessage(const Message& aMessage);
		void SendInstantMessage(const Message& aMessage);
		void SendSavedMessages();

	private:
		std::vector<Message> myMessages;
		std::unordered_multimap<eMessageType, Observer*> myObservers;

		void SendMessageToSubscribers(const Message& aMessage) const;
	};
}