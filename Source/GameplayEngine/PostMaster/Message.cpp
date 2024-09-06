#include "Message.h"

Crimson::Message::Message(eMessageType aType) :myMessageType(aType), myData()
{
}

Crimson::Message::Message(eMessageType aType, bool aValue) :myMessageType(aType), myData(aValue)
{
}

Crimson::Message::Message(eMessageType aType, int aValue) :myMessageType(aType), myData(aValue)
{
}

Crimson::Message::Message(eMessageType aType, float aValue) :myMessageType(aType), myData(aValue)
{
}

Crimson::Message::Message(eMessageType aType, const std::string& aValue) :myMessageType(aType), myData(aValue)
{
}

Crimson::Message::Message(eMessageType aType, const Vector2<float>& aValue) :myMessageType(aType), myData(aValue)
{
}

Crimson::Message::Message(eMessageType aType, const Vector3<float>& aValue) :myMessageType(aType), myData(aValue)
{
}

Crimson::eMessageType Crimson::Message::GetMessageType() const
{
	return myMessageType;
}

const bool* Crimson::Message::GetDataAsBool() const
{
	return std::get_if<bool>(&myData);
}

const int* Crimson::Message::GetDataAsInt() const
{
	return std::get_if<int>(&myData);
}

const float* Crimson::Message::GetDataAsFloat() const
{
	return std::get_if<float>(&myData);
}

const std::string* Crimson::Message::GetDataAsString() const
{
	return std::get_if<std::string>(&myData);
}

const Crimson::Vector2<float>* Crimson::Message::GetDataAsVector2() const
{
	return std::get_if<Vector2<float>>(&myData);
}

const Crimson::Vector3<float>* Crimson::Message::GetDataAsVector3() const
{
	return std::get_if<Vector3<float>>(&myData);
}
