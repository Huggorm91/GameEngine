#include "AssetManager.pch.h"
#include "Component.h"
#include "../GameObject.h"

Component::Component() : myParent(nullptr), myIsActive(true), myType(ComponentType::Unknown)
{
}

Component::Component(ComponentType aType) : myParent(nullptr), myIsActive(true), myType(aType)
{
}

Component::Component(const Component& aComponent) : myParent(aComponent.myParent), myIsActive(aComponent.myIsActive), myType(aComponent.myType)
{
}

Component::Component(const Json::Value& aJson) : myParent(nullptr), myIsActive(aJson["IsActive"].asBool()), myType(static_cast<ComponentType>(aJson["Type"].asInt()))
{
}

Component& Component::operator=(const Component& aComponent)
{
	myParent = aComponent.myParent;
	myIsActive = aComponent.myIsActive;
	myType = aComponent.myType;
	return *this;
}

void Component::Init(const Json::Value& aJson)
{
	myIsActive = aJson["IsActive"].asBool();
	myType = static_cast<ComponentType>(aJson["Type"].asInt());
}

void Component::Init(GameObject* aParent)
{
	myParent = aParent;
}

void Component::Update()
{
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

void Component::ComponentPointersInvalidated()
{
}

Json::Value Component::ToJson() const
{
	Json::Value result;
	result["IsActive"] = myIsActive;
	result["Type"] = static_cast<int>(myType);
#ifdef _DEBUG
	result["TypeName"] = ComponentTypeToString(myType);
#endif // _DEBUG

	return result;
}

const Component* Component::GetTypePointer() const
{
	return this;
}

const CommonUtilities::Blackboard<unsigned int>& Component::GetComponentContainer() const
{
	assert(myParent != nullptr && "Component not Initialized!");
	return myParent->myComponents;
}

CommonUtilities::Blackboard<unsigned int>& Component::GetComponentContainer()
{
	assert(myParent != nullptr && "Component not Initialized!");
	return myParent->myComponents;
}

void Component::NotifyInput(CommonUtilities::eInputAction anEvent)
{
}
