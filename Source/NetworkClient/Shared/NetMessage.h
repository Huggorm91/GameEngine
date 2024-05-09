#pragma once
#include "Globals.h"

namespace MessageType
{
	enum Enum: short
	{
		Invalid,
		Connect,
		Disconnect,
		Confirmation,
		Ping,
		Message
	};
};

struct NetMessage
{
	MessageType::Enum type = MessageType::Invalid;
	unsigned short dataSize = 0;
	bool needReply = false;
	char data[globalBuffLength]{'\0'};

	inline operator char* ()
	{
		return reinterpret_cast<char*>(this);
	}
	inline operator const char* () const
	{
		return reinterpret_cast<const char*>(this);
	}
};