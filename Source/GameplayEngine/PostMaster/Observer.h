#pragma once

namespace Crimson
{
	class Message;
}

class Observer
{
public:
	virtual ~Observer();

	virtual void RecieveMessage(const Crimson::Message& aMessage) = 0;
};