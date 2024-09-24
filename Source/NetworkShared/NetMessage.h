#pragma once
#include "Globals.h"

namespace Network
{
	enum class MessageType : short
	{
		Invalid,
		Connect,
		Disconnect,
		Confirmation,
		Chat,
		Ping,
		GameObjectMessage,
		CreateGameObject,
		DeleteGameObject,

		Count
	};

	struct NetMessage
	{
		MessageType type = MessageType::Invalid;
		unsigned short senderID = 0;
		unsigned short messageID = 0;
		unsigned short dataSize = 0;
		unsigned short packetIndex = 0;
		unsigned short totalPackets = 1;
		bool needReply = false;
		char data[globalBuffLength]{ '\0' };

		inline operator char* ()
		{
			return reinterpret_cast<char*>(this);
		}
		inline operator const char* () const
		{
			return reinterpret_cast<const char*>(this);
		}

		inline int GetCurrentSize() const
		{
			constexpr int staticSize = sizeof(NetMessage) - sizeof(NetMessage::data);
			return staticSize + dataSize;
		}
	};

	static_assert(sizeof(NetMessage) <= 576, "NetMessage is too large to fit safely in an UDP message");
}