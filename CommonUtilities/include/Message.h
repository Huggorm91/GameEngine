#pragma once
#include <variant>
#include <string>
#include "Vector3.hpp"
#include "Enums/EnumMessageTypes.h"

namespace CommonUtilities
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
		Message(eMessageType aType, const Vector2<float>& aValue);
		Message(eMessageType aType, const Vector3<float>& aValue);

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
		const Vector2<float>* GetDataAsVector2() const;

		// Returns nullptr if incorrect type
		const Vector3<float>* GetDataAsVector3() const;

	private:
		eMessageType myMessageType;
		std::variant<bool, int, float, std::string, Vector2<float>, Vector3<float>> myData;
	};

	template<typename T>
	inline bool Message::HasDataType() const
	{
		return std::holds_alternative<T>(myData);
	}
}