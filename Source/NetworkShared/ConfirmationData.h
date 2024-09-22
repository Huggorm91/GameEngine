#pragma once
#include "NetMessage.h"

namespace Network
{
	struct ConfirmationData
	{
		NetMessage message;
		float timeSinceLastSend = 0.f;
		unsigned char amountSent = 0u;

		inline bool operator==(const NetMessage& aMessage) const
		{
			return aMessage.senderID == message.senderID && aMessage.messageID == message.messageID && aMessage.packetIndex == message.packetIndex;
		}
	};
}