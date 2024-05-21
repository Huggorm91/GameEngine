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

	NetMessage CreateMoveGameObjectMessage(unsigned anID, const Crimson::Vector3<float>& aPosition, const Crimson::Vector3<float>& aRotation)
	{
		NetMessage message;
		message.type = MessageType::MoveGameObject;
		constexpr unsigned short size = sizeof(unsigned) + sizeof(Crimson::Vector3<float>) + sizeof(Crimson::Vector3<float>);
		message.dataSize = size;

		memcpy_s(message.data, globalBuffLength, &anID, sizeof(unsigned));

		constexpr unsigned firstOffset = sizeof(unsigned);
		memcpy_s(message.data + firstOffset, globalBuffLength, &aPosition, sizeof(Crimson::Vector3<float>));

		constexpr unsigned secondOffset = sizeof(unsigned) + sizeof(Crimson::Vector3<float>);
		memcpy_s(message.data + secondOffset, globalBuffLength, &aRotation, sizeof(Crimson::Vector3<float>));
		return message;
	}
}