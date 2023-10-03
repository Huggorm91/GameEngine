#include "PostMaster.h"

Crimson::PostMaster* Crimson::PostMaster::myInstance = nullptr;

Crimson::PostMaster::PostMaster() : myObservers(), myMessages()
{
}

Crimson::PostMaster::~PostMaster()
{
	delete myInstance;
}

void Crimson::PostMaster::Subscribe(Observer* anObserver, const eMessageType aType)
{
	myObservers.emplace(aType, anObserver);
}

void Crimson::PostMaster::UnsubscribeAll()
{
	myObservers.clear();
}

void Crimson::PostMaster::UnsubscribeFromAllMessages(Observer* anObserver)
{
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
	myMessages.emplace_back(aMessage);
}

void Crimson::PostMaster::SendInstantMessage(const Message& aMessage)
{
	SendMessageToSubscribers(aMessage);
}

void Crimson::PostMaster::SendSavedMessages()
{
	for (auto& message : myMessages)
	{
		SendMessageToSubscribers(message);
	}
	myMessages.clear();
}

Crimson::PostMaster* Crimson::PostMaster::GetInstance()
{
	if (myInstance == nullptr)
	{
		myInstance = new PostMaster();
	}
	return myInstance;
}

void Crimson::PostMaster::SendMessageToSubscribers(const Message& aMessage) const
{
	auto range = myObservers.equal_range(aMessage.GetMessageType());
	for (auto iter = range.first; iter != range.second; iter++)
	{
		iter->second->RecieveMessage(aMessage);
	}
}