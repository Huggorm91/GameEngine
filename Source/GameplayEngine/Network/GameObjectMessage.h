#pragma once
#include "NetworkShared/Globals.h"
#pragma warning (push,0)
#include "CrimsonUtilities/UUID/uuid_v4.h"
#pragma warning (pop)

namespace Network
{
	enum class ObjectAction: unsigned short
	{
		Invalid,
		Move,
	};

	constexpr unsigned short CalculateGameObjectMessageNonDataSize() { return sizeof(UUIDv4::UUID) + sizeof(ObjectAction) + sizeof(unsigned short); }

	struct GameObjectMessage
	{
		UUIDv4::UUID id;
		ObjectAction action;
		unsigned short size;
		char data[globalBuffLength - CalculateGameObjectMessageNonDataSize()]{'\0'};

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