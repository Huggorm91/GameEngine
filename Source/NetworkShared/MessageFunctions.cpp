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
}