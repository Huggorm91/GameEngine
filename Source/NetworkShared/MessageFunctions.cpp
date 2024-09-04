#include "MessageFunctions.h"
#include "CrimsonUtilities/Math/Vector3.hpp"
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

	NetMessage CreateGameObjectMessage(const UUIDv4::UUID& anID, const GameObjectMessage& aMessage)
	{
		static_assert(sizeof(UUIDv4::UUID) + sizeof(GameObjectMessage) == globalBuffLength, "Size of GameObjectMessage does not match 'globalBuffLength'!");

		constexpr rsize_t idSize = sizeof(UUIDv4::UUID);
		constexpr rsize_t messageSize = sizeof(GameObjectMessage);
		constexpr unsigned short size = idSize + messageSize;

		NetMessage message;
		message.dataSize = size;
		message.type = MessageType::GameObjectMessage;

		memcpy_s(message.data, globalBuffLength, &anID, idSize);
		memcpy_s(message.data + idSize, globalBuffLength, aMessage, messageSize);

		return message;
	}

	const GameObjectMessage& ExtractGameObjectMessage(const NetMessage& aMessage)
	{
		static_assert(sizeof(UUIDv4::UUID) + sizeof(GameObjectMessage) == globalBuffLength, "Size of GameObjectMessage does not match 'globalBuffLength'!");
		assert(aMessage.type == MessageType::GameObjectMessage && "Invalid MessageType!");

		constexpr rsize_t idSize = sizeof(UUIDv4::UUID);
		return reinterpret_cast<const GameObjectMessage&>(aMessage.data[idSize]);
	}

	NetMessage CreateMoveGameObjectMessage(const UUIDv4::UUID& anID, const Crimson::Vector3f& aPosition, const Crimson::Vector3f& aRotation)
	{
		constexpr rsize_t dataSize = sizeof(GameObjectMessage::data);
		constexpr rsize_t vectorSize = sizeof(Crimson::Vector3f);

		GameObjectMessage message;
		message.action = ObjectAction::Move;

		memcpy_s(message.data, dataSize, &aPosition, vectorSize);
		memcpy_s(message.data + vectorSize, dataSize, &aRotation, vectorSize);

		return CreateGameObjectMessage(anID, message);
	}
}