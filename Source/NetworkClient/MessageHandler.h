#pragma once
#include <vector>
#include <string>
#include "NetworkShared/NetMessage.h"

namespace Network
{
	class Client;
	class Transform;

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

		void SendTransformChanged(const Transform& aTransform, unsigned anID);

		void SendChatMessage(const std::string& aMessage);
		constexpr std::string GetSelfHeader() const;
		const std::vector<std::string>& GetChatHistory() const;

	private:
		std::vector<std::string> myChatHistory;
		std::vector<NetMessage> myMessages;
		Client* myClient;
	};
}