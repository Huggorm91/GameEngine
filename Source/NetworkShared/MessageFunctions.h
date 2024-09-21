#pragma once
#pragma warning (push,0)
#include "CrimsonUtilities/UUID/uuid_v4.h"
#pragma warning (pop)
#include "NetMessage.h"
#include <string>

namespace Crimson
{
	template <typename T>
	class Vector3;
}

namespace Network
{
	struct GameObjectMessage;

	NetMessage CreateConnectMessage(bool aNeedReply, const std::string& aUserName);
	NetMessage CreateDisconnectMessage();

	NetMessage CreateConfirmationMessage();
	NetMessage CreateChatMessage(const std::string& aMessage);

	NetMessage CreateGameObjectMessage(const GameObjectMessage& aMessage);

	constexpr unsigned short GetMaximumCreateGameobjectDataSize() { return globalBuffLength - sizeof(UUIDv4::UUID); }
	NetMessage CreateCreateGameObjectMessage(const UUIDv4::UUID& anID, const std::vector<uint8_t>& someData);

	NetMessage CreateDeleteGameObjectMessage(const UUIDv4::UUID& anID);

	const UUIDv4::UUID& ExtractUUID(const NetMessage& aMessage);
	const GameObjectMessage& ExtractGameObjectMessage(const NetMessage& aMessage);

	inline bool MultiMessageSort(const NetMessage& aFirst, const NetMessage& aSecond)
	{
		if (aFirst.senderID == aSecond.senderID)
		{
			if (aFirst.messageID == aSecond.messageID)
			{
				return aFirst.packetIndex < aSecond.packetIndex;
			}
			else
			{
				return aFirst.messageID < aSecond.messageID;
			}
		}
		else
		{
			return aFirst.senderID < aSecond.senderID;
		}
	}
}