#include "PostMaster.h"

void Crimson::PostMaster::Subscribe(Observer* anObserver, const eMessageType aType)
{
	if (myMutex.try_lock())
	{
		myObservers.emplace(aType, anObserver);
		myMutex.unlock();
	}
	else
	{
		std::unique_lock lock(mySecondaryMutex);
		myAddList.emplace_back(std::pair(eMessageType::Count, anObserver));
	}
}

void Crimson::PostMaster::ClearSubscribers()
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

void Crimson::PostMaster::UnsubscribeFromAllMessages(Observer* anObserver)
{
	if (myMutex.try_lock())
	{
		RemoveSubscriber(eMessageType::Count, anObserver);
		myMutex.unlock();
	}
	else
	{
		std::unique_lock lock(mySecondaryMutex);
		myDeleteList.emplace_back(std::pair(eMessageType::Count, anObserver));
	}
}

void  Crimson::PostMaster::UnsubscribeFromMessage(const  Crimson::eMessageType& aMessageType, Observer* anObserver)
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

void Crimson::PostMaster::AddMessage(const Message& aMessage)
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

void Crimson::PostMaster::SendInstantMessage(const Message& aMessage)
{
	std::shared_lock lock(myMutex);
	SendMessageToSubscribers(aMessage);
}

void Crimson::PostMaster::SendSavedMessages()
{
	std::shared_lock lock(myMutex);
	for (auto& message : myMessages)
	{
		SendMessageToSubscribers(message);
	}
	myMessages.clear();

	if (!mySecondaryMessages.empty() || !myDeleteList.empty() || !myAddList.empty())
	{
		std::unique_lock lock2(mySecondaryMutex);

		if (!mySecondaryMessages.empty())
		{
			myMessages.swap(mySecondaryMessages);
		}
		
		if (!myDeleteList.empty())
		{
			for (auto& [type, observer] : myDeleteList)
			{
				RemoveSubscriber(type, observer);
			}
			myDeleteList.clear();
		}

		if (!myAddList.empty())
		{
			for (auto& [type, observer] : myAddList)
			{
				myObservers.emplace(type, observer);
			}
			myAddList.clear();
		}
	}
}

void Crimson::PostMaster::SendMessageToSubscribers(const Message& aMessage) const
{
	auto range = myObservers.equal_range(aMessage.GetMessageType());
	for (auto iter = range.first; iter != range.second; iter++)
	{
		iter->second->RecieveMessage(aMessage);
	}
}

void Crimson::PostMaster::RemoveSubscriber(const eMessageType& aMessageType, Observer* anObserver)
{
	if (aMessageType == eMessageType::Count) 
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
