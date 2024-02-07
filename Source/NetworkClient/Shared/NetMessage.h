#pragma once
#include "Globals.h"

struct MessageType
{
	enum Enum
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
	bool needReply;
	unsigned short dataSize = 0;
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