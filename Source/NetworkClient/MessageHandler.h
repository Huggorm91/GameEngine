#pragma once
#include <vector>
#include <string>
#include "NetworkShared/GameObjectMessage.h"

class Transform;

namespace Network
{
	class Client;

	class MessageHandler
	{
	public:
		MessageHandler();
		~MessageHandler();

		void Init();
		void Update();

		void Connect();
		bool IsConnected() const;

		void SendNetMessage(const NetMessage& aMessage) const;

		void SendTransformChanged(const Transform& aTransform, const UUIDv4::UUID& anID);

		void SendChatMessage(const std::string& aMessage);
		constexpr std::string GetSelfHeader() const;

		const std::vector<std::string>& GetChatHistory() const;
		std::vector<NetMessage>& GetMessages();

	private:
		std::vector<std::string> myChatHistory;
		std::vector<NetMessage> myMessages;
		std::unique_ptr<Client> myClient;
	};
}