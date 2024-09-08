#include "PostMaster.h"

void PostMaster::Subscribe(Observer* anObserver, const Crimson::eMessageType aType)
{
	if (myMutex.try_lock())
	{
		myObservers.emplace(aType, anObserver);
		myMutex.unlock();
	}
	else
	{
		std::unique_lock lock(mySecondaryMutex);
		myAddList.emplace_back(std::pair(aType, anObserver));
	}
}

void PostMaster::ClearSubscribers()
{
	if (myMutex.try_lock())
	{
		myObservers.clear();
		myMutex.unlock();
	}
	else
	{
		std::shared_lock lock(myMutex);
		std::unique_lock lock2(mySecondaryMutex);
		for (auto& [type, observer] : myObservers)
		{
			myDeleteList.emplace_back(std::pair(type, observer));
		}
	}
}

void PostMaster::UnsubscribeFromAllMessages(Observer* anObserver)
{
	if (myMutex.try_lock())
	{
		RemoveSubscriber(Crimson::eMessageType::Count, anObserver);
		myMutex.unlock();
	}
	else
	{
		std::unique_lock lock(mySecondaryMutex);
		myDeleteList.emplace_back(std::pair(Crimson::eMessageType::Count, anObserver));
	}
}

void  PostMaster::UnsubscribeFromMessage(const  Crimson::eMessageType& aMessageType, Observer* anObserver)
{
	if (myMutex.try_lock())
	{
		RemoveSubscriber(aMessageType, anObserver);
		myMutex.unlock();
	}
	else
	{
		std::unique_lock lock(mySecondaryMutex);
		myDeleteList.emplace_back(std::pair(aMessageType, anObserver));
	}
}

void PostMaster::AddMessage(const Crimson::Message& aMessage)
{
	if (myMutex.try_lock())
	{
		myMessages.emplace_back(aMessage);
		myMutex.unlock();
	}
	else
	{
		std::unique_lock lock(mySecondaryMutex);
		mySecondaryMessages.emplace_back(aMessage);
	}
}

void PostMaster::SendInstantMessage(const Crimson::Message& aMessage)
{
	std::shared_lock lock(myMutex);
	
	if (mySecondaryMutex.try_lock())
	{
		// Try sending to those added this frame
		for (auto& [type, observer] : myAddList)
		{
			if (type == aMessage.GetMessageType())
			{
				observer->RecieveMessage(aMessage);
			}
		}
		mySecondaryMutex.unlock();
	}

	SendMessageToSubscribers(aMessage);
}

void PostMaster::SendSavedMessages()
{
	std::shared_lock lock(myMutex);
	if (!mySecondaryMessages.empty() || !myDeleteList.empty() || !myAddList.empty())
	{
		std::unique_lock lock2(mySecondaryMutex);

		if (!mySecondaryMessages.empty())
		{
			myMessages.insert(myMessages.end(), mySecondaryMessages.begin(), mySecondaryMessages.end());
			mySecondaryMessages.clear();
		}

		if (!myAddList.empty())
		{
			for (auto& [type, observer] : myAddList)
			{
				myObservers.emplace(type, observer);
			}
			myAddList.clear();
		}

		if (!myDeleteList.empty())
		{
			for (auto& [type, observer] : myDeleteList)
			{
				RemoveSubscriber(type, observer);
			}
			myDeleteList.clear();
		}
	}

	for (auto& message : myMessages)
	{
		SendMessageToSubscribers(message);
	}
	myMessages.clear();
}

void PostMaster::SendMessageToSubscribers(const Crimson::Message& aMessage) const
{
	auto range = myObservers.equal_range(aMessage.GetMessageType());
	for (auto iter = range.first; iter != range.second; iter++)
	{
		iter->second->RecieveMessage(aMessage);
	}
}

void PostMaster::RemoveSubscriber(const Crimson::eMessageType& aMessageType, Observer* anObserver)
{
	if (aMessageType == Crimson::eMessageType::Count)
	{
		// Remove from all types
		auto iterator = myObservers.begin();
		while (iterator != myObservers.end())
		{
			if (iterator->second == anObserver)
			{
				iterator = myObservers.erase(iterator);
			}
			else
			{
				iterator++;
			}
		}
	}
	else
	{
		auto range = myObservers.equal_range(aMessageType);
		for (auto iterator = range.first; iterator != range.second; ++iterator)
		{
			if (iterator->second == anObserver)
			{
				myObservers.erase(iterator);
				return;
			}
		}
	}
}