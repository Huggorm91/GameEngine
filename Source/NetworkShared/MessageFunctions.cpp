#include "MessageFunctions.h"
#include "CrimsonUtilities/Math/Vector3.hpp"
#include "GameplayEngine/Network/GameObjectMessage.h"

namespace Network
{
	NetMessage CreateConnectMessage(bool aNeedReply, const std::string& aUserName)
	{
		NetMessage message;
		message.type = MessageType::Connect;
		message.needReply = aNeedReply;
		message.dataSize = static_cast<unsigned short>(aUserName.size() + 1);
		strcpy_s(message.data, message.dataSize, aUserName.c_str());
		return message;
	}

	NetMessage CreateDisconnectMessage()
	{
		NetMessage message;
		message.type = MessageType::Disconnect;
		return message;
	}

	NetMessage CreatePingMessage()
	{
		NetMessage message;
		message.type = MessageType::Ping;
		message.needReply = true;
		return message;
	}

	NetMessage CreateConfirmationMessage()
	{
		NetMessage message;
		message.type = MessageType::Confirmation;
		return message;
	}

	NetMessage CreateChatMessage(const std::string& aMessage)
	{
		NetMessage message;
		message.type = MessageType::Chat;
		message.dataSize = static_cast<unsigned short>(aMessage.size() + 1);
		strcpy_s(message.data, message.dataSize, aMessage.c_str());
		return message;
	}

	NetMessage CreateGameObjectMessage(const GameObjectMessage& aMessage)
	{
		static_assert(sizeof(GameObjectMessage) == globalBuffLength, "Size of GameObjectMessage does not match 'globalBuffLength'!");
		static_assert(CalculateGameObjectMessageNonDataSize() + sizeof(GameObjectMessage::data) == sizeof(GameObjectMessage), "Size of GameObjectMessage.data + CalculateGameObjectMessageNonDataSize() does not match size of GameObjectMessage!");

		constexpr rsize_t messageSize = sizeof(GameObjectMessage);

		NetMessage message;
		message.dataSize = CalculateGameObjectMessageNonDataSize() + aMessage.size;
		message.type = MessageType::GameObjectMessage;

		memcpy_s(message.data, globalBuffLength, aMessage, messageSize);

		return message;
	}

	NetMessage CreateCreateGameObjectMessage(const UUIDv4::UUID& anID, const std::vector<uint8_t>& someData)
	{
		constexpr rsize_t idSize = sizeof(UUIDv4::UUID);
		assert(someData.size() <= GetMaximumCreateGameobjectDataSize() && "The data is too large to be packed into a NetMessage!");

		NetMessage message;
		message.dataSize = static_cast<unsigned short>(idSize + someData.size());
		message.type = MessageType::CreateGameObject;

		memcpy_s(message.data, globalBuffLength, &anID, idSize);
		memcpy_s(message.data + idSize, globalBuffLength - idSize, someData.data(), someData.size());

		return message;
	}

	NetMessage CreateDeleteGameObjectMessage(const UUIDv4::UUID& anID)
	{
		constexpr rsize_t idSize = sizeof(UUIDv4::UUID);

		NetMessage message;
		message.dataSize = idSize;
		message.type = MessageType::DeleteGameObject;

		memcpy_s(message.data, globalBuffLength, &anID, idSize);

		return message;
	}

	UUIDv4::UUID ExtractUUID(const NetMessage& aMessage)
	{
		assert((aMessage.type == MessageType::GameObjectMessage 
			|| aMessage.type == MessageType::CreateGameObject
			|| aMessage.type == MessageType::DeleteGameObject
			|| aMessage.type == MessageType::Confirmation) // Only Confirmations that was previously any of the above types is actually valid
			&& "Invalid MessageType!");
		return reinterpret_cast<const UUIDv4::UUID&>(aMessage.data);
	}

	const GameObjectMessage& ExtractGameObjectMessage(const NetMessage& aMessage)
	{
		static_assert(sizeof(GameObjectMessage) == globalBuffLength, "Size of GameObjectMessage does not match 'globalBuffLength'!");
		assert(aMessage.type == MessageType::GameObjectMessage && "Invalid MessageType!");

		return reinterpret_cast<const GameObjectMessage&>(aMessage.data);
	}
}