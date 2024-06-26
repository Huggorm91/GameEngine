#pragma once

namespace Crimson
{
	class Message;

	class Observer
	{
	public:
		virtual ~Observer() = default;

		virtual void RecieveMessage(const Message& aMessage) = 0;
	private:
	};
}