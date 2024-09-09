#include "AssetManager.pch.h"
#include "Component.h"
#include "../GameObject.h"

Component::Component() : myParent(nullptr), myIsActive(true), myType(ComponentType::Unknown)
{}

Component::Component(ComponentType aType) : myParent(nullptr), myIsActive(true), myType(aType)
{}

Component::Component(const Component& aComponent) : myParent(aComponent.myParent), myIsActive(aComponent.myIsActive), myType(aComponent.myType)
{}

Component::Component(Component&& aComponent) noexcept : myParent(aComponent.myParent), myIsActive(aComponent.myIsActive), myType(aComponent.myType)
{}

Component::Component(const Json::Value& aJson) : myParent(nullptr), myIsActive(aJson["IsActive"].asBool()), myType(static_cast<ComponentType>(aJson["Type"].asInt()))
{}

Component& Component::operator=(const Component& aComponent)
{
	myParent = aComponent.myParent;
	myIsActive = aComponent.myIsActive;
	myType = aComponent.myType;
	return *this;
}

Component& Component::operator=(Component&& aComponent) noexcept
{
	myParent = aComponent.myParent;
	myIsActive = aComponent.myIsActive;
	myType = aComponent.myType;
	return *this;
}

void Component::Init(GameObject* aParent)
{
	myParent = aParent;
}

void Component::SetParent(GameObject* aParent)
{
	myParent = aParent;
}

const GameObject& Component::GetParent() const
{
	return *myParent;
}

GameObject& Component::GetParent()
{
	return *myParent;
}

ComponentType Component::GetType() const
{
	return myType;
}

const UUIDv4::UUID& Component::GetParentID() const
{
	return myParent->myUUID;
}

void Component::SetActive(bool aIsActive)
{
	myIsActive = aIsActive;
}

void Component::ToogleActive()
{
	myIsActive = !myIsActive;
}

bool Component::IsActive() const
{
	return myIsActive;
}

#ifdef EDITOR
void Component::CreateImGuiComponents(const std::string&)
{
	if (ImGui::Checkbox("Active", &myIsActive))
	{
		SetActive(myIsActive);
	}
}
#endif // EDITOR

Json::Value Component::ToJson() const
{
	Json::Value result;
	result["IsActive"] = myIsActive;
	result["Type"] = static_cast<int>(myType);
#ifdef _DEBUG
	result["TypeName"] = ToString();
#endif // _DEBUG

	return result;
}

void Component::Serialize(std::ostream& aStream) const
{
	Binary::eType type = Binary::Component;
	aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	aStream.write(reinterpret_cast<const char*>(&myType), sizeof(myType));
	aStream.write(reinterpret_cast<const char*>(&myIsActive), sizeof(myIsActive));
}

void Component::Deserialize(std::istream& aStream)
{
	aStream.read(reinterpret_cast<char*>(&myIsActive), sizeof(myIsActive));
}

const Transform* Component::GetParentTransform() const
{
	assert(myParent != nullptr && "Component not Initialized!");
	return &myParent->myTransform;
}

Transform* Component::GetParentTransform()
{
	assert(myParent != nullptr && "Component not Initialized!");
	return &myParent->myTransform;
}