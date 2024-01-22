#include "PostMaster.h"

void Crimson::PostMaster::Subscribe(Observer* anObserver, const eMessageType aType)
{
	std::unique_lock lock(myMutex);
	myObservers.emplace(aType, anObserver);
}

void Crimson::PostMaster::ClearSubscribers()
{
	std::unique_lock lock(myMutex);
	myObservers.clear();
}

void Crimson::PostMaster::UnsubscribeFromAllMessages(Observer* anObserver)
{
	std::unique_lock lock(myMutex);
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

void  Crimson::PostMaster::UnsubscribeFromMessage(const  Crimson::eMessageType& aMsgType, Observer* anObserver)
{
	std::unique_lock lock(myMutex);
	auto range = myObservers.equal_range(aMsgType);
	for (auto iterator = range.first; iterator != range.second; ++iterator)
	{
		if (iterator->second == anObserver)
		{
			myObservers.erase(iterator);
			return;
		}
	}
}

void Crimson::PostMaster::AddMessage(const Message& aMessage)
{
	std::unique_lock lock(myMutex);
	myMessages.emplace_back(aMessage);
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
}

void Crimson::PostMaster::SendMessageToSubscribers(const Message& aMessage) const
{
	auto range = myObservers.equal_range(aMessage.GetMessageType());
	for (auto iter = range.first; iter != range.second; iter++)
	{
		iter->second->RecieveMessage(aMessage);
	}
}