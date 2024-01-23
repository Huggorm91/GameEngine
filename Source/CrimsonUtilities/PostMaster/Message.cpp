#include "Message.h"

Crimson::Message::Message(eMessageType aType) :myMessageType(aType), myData()
{}

Crimson::Message::Message(eMessageType aType, bool aValue) :myMessageType(aType), myData(aValue)
{}

Crimson::Message::Message(eMessageType aType, int aValue) :myMessageType(aType), myData(aValue)
{}

Crimson::Message::Message(eMessageType aType, float aValue) :myMessageType(aType), myData(aValue)
{}

Crimson::Message::Message(eMessageType aType, const std::string& aValue) :myMessageType(aType), myData(aValue)
{}

Crimson::Message::Message(eMessageType aType, const Vector2i& aValue) :myMessageType(aType), myData(aValue)
{}

Crimson::Message::Message(eMessageType aType, const Vector2f& aValue) :myMessageType(aType), myData(aValue)
{}

Crimson::Message::Message(eMessageType aType, const Vector3f& aValue) :myMessageType(aType), myData(aValue)
{}

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

const Crimson::Vector2i* Crimson::Message::GetDataAsVector2i() const
{
	return std::get_if<Vector2i>(&myData);
}

const Crimson::Vector2f* Crimson::Message::GetDataAsVector2() const
{
	return std::get_if<Vector2f>(&myData);
}

const Crimson::Vector3f* Crimson::Message::GetDataAsVector3() const
{
	return std::get_if<Vector3f>(&myData);
}
