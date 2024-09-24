#pragma once
#include "NetMessage.h"

namespace Network
{
	struct ConfirmationData
	{
		NetMessage message;
		float timeSinceLastSend;
		bool shouldLimitRetries;
		unsigned char amountSent;

		ConfirmationData() : message(), timeSinceLastSend(0.f), shouldLimitRetries(true), amountSent(0u) {}
		ConfirmationData(const NetMessage& aMessage, bool aShouldLimitRetries = true) : message(aMessage), timeSinceLastSend(0.f), shouldLimitRetries(aShouldLimitRetries), amountSent(0u) {}

		inline bool operator==(const NetMessage& aMessage) const
		{
			return aMessage.senderID == message.senderID && aMessage.messageID == message.messageID && aMessage.packetIndex == message.packetIndex;
		}
	};
}