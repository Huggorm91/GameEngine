#pragma once
#include "Observer.hpp"
#include "Message.h"
#include <vector>
#include <unordered_map>

namespace Crimson
{
	class PostMaster
	{
	public:
		~PostMaster();

		void Subscribe(Observer* anObserver, const eMessageType aType);

		void UnsubscribeAll();
		void UnsubscribeFromAllMessages(Observer* anObserver);
		void UnsubscribeFromMessage(const eMessageType& aMsgType, Observer* anObserver);

		void AddMessage(const Message& aMessage);
		void SendInstantMessage(const Message& aMessage);
		void SendSavedMessages();

		static PostMaster* GetInstance();

	private:
		std::vector<Message> myMessages;
		std::unordered_multimap<eMessageType, Observer*> myObservers;
		static PostMaster* myInstance;

		PostMaster();

		void SendMessageToSubscribers(const Message& aMessage) const;
	};
}