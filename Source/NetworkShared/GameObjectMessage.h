#pragma once
#include "NetMessage.h"
#pragma warning (push,0)
#include "CrimsonUtilities/UUID/uuid_v4.h"
#pragma warning (pop)

namespace Network
{
	enum class ObjectAction
	{
		Invalid,
		Move,
	};

	struct GameObjectMessage
	{
		ObjectAction action;
		char data[globalBuffLength - (sizeof(UUIDv4::UUID) + sizeof(ObjectAction))]{ '\0' };

		inline operator char* ()
		{
			return reinterpret_cast<char*>(this);
		}
		inline operator const char* () const
		{
			return reinterpret_cast<const char*>(this);
		}
	};
}