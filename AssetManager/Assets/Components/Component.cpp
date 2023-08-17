#include "AssetManager.pch.h"
#include "Component.h"
#include "../GameObject.h"

Component::Component(): myParent(nullptr), myIsActive(true)
{

}

Component::Component(const Component& aComponent): myParent(aComponent.myParent), myIsActive(aComponent.myIsActive)
{
}

Component& Component::operator=(const Component& aComponent)
{
	myParent = aComponent.myParent;
	myIsActive = aComponent.myIsActive;
	return *this;
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
