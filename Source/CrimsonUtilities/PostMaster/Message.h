#pragma once
#include <variant>
#include <string>
#include "Math/Vector3.hpp"
#include "EnumMessageTypes.h"

namespace Crimson
{
	class Message
	{
	public:
		Message() = delete;
		Message(eMessageType aType);
		Message(eMessageType aType, bool aValue);
		Message(eMessageType aType, int aValue);
		Message(eMessageType aType, float aValue);
		Message(eMessageType aType, const std::string& aValue);
		Message(eMessageType aType, const Vector2i& aValue);
		Message(eMessageType aType, const Vector2f& aValue);
		Message(eMessageType aType, const Vector3f& aValue);

		eMessageType GetMessageType() const;

		// Example: HasDataType<int>()
		template <typename T>
		bool HasDataType() const;

		// Returns nullptr if incorrect type
		const bool* GetDataAsBool() const;

		// Returns nullptr if incorrect type
		const int* GetDataAsInt() const;

		// Returns nullptr if incorrect type
		const float* GetDataAsFloat() const;

		// Returns nullptr if incorrect type
		const std::string* GetDataAsString() const;

		// Returns nullptr if incorrect type
		const Vector2i* GetDataAsVector2i() const;

		// Returns nullptr if incorrect type
		const Vector2f* GetDataAsVector2() const;

		// Returns nullptr if incorrect type
		const Vector3f* GetDataAsVector3() const;

	private:
		eMessageType myMessageType;
		std::variant<bool, int, float, std::string, Vector2i, Vector2f, Vector3f> myData;
	};

	template<typename T>
	inline bool Message::HasDataType() const
	{
		return std::holds_alternative<T>(myData);
	}
}