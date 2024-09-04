#pragma once

#include "GameObjectMessage.h"
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

	NetMessage CreateCreateGameObjectMessage(const UUIDv4::UUID& anID);
	NetMessage CreateDeleteGameObjectMessage(const UUIDv4::UUID& anID);

	NetMessage CreateGameObjectMessage(const UUIDv4::UUID& anID, const GameObjectMessage& aMessage);
	NetMessage CreateMoveGameObjectMessage(const UUIDv4::UUID& anID, const Crimson::Vector3<float>& aPosition, const Crimson::Vector3<float>& aRotation);

	const GameObjectMessage& ExtractGameObjectMessage(const NetMessage& aMessage);
}