#pragma once
#include "NetMessage.h"
#include <string>

namespace Crimson
{
	template <typename T>
	class Vector3;
}

namespace Network
{
	NetMessage CreateConnectMessage(bool aNeedReply, const std::string& aUserName);
	NetMessage CreateDisconnectMessage();

	NetMessage CreateConfirmationMessage();
	NetMessage CreatePingMessage(bool aNeedReply);
	NetMessage CreateChatMessage(const std::string& aMessage);

	NetMessage CreateMoveGameObjectMessage(unsigned anID, const Crimson::Vector3<float>& aNewPosition);
	NetMessage CreateCreateGameObjectMessage(unsigned anID);
	NetMessage CreateDeleteGameObjectMessage(unsigned anID);
}