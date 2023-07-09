#pragma once

namespace CommonUtilities
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