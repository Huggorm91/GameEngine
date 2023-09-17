#include "AssetManager.pch.h"
#include "Component.h"
#include "../GameObject.h"

unsigned int Component::localIDCount = 0;

Component::Component() : myParent(nullptr), myIsActive(true), myType(ComponentType::Unknown), myID(localIDCount++)
{
}

Component::Component(ComponentType aType) : myParent(nullptr), myIsActive(true), myType(aType), myID(localIDCount++)
{
}

Component::Component(const Component& aComponent) : myParent(aComponent.myParent), myIsActive(aComponent.myIsActive), myType(aComponent.myType), myID(localIDCount++)
{
}

Component::Component(Component&& aComponent) noexcept : myParent(aComponent.myParent), myIsActive(aComponent.myIsActive), myType(aComponent.myType), myID(aComponent.myID)
{
}

Component::Component(const Json::Value& aJson) : myParent(nullptr), myIsActive(aJson["IsActive"].asBool()), myType(static_cast<ComponentType>(aJson["Type"].asInt())), myID(aJson["ID"].asUInt())
{
}

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

void Component::Init(const Json::Value& aJson)
{
	myIsActive = aJson["IsActive"].asBool();
	myType = static_cast<ComponentType>(aJson["Type"].asInt());
	const_cast<unsigned&>(myID) = aJson["ID"].asUInt();
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

unsigned Component::GetID() const
{
	return myID;
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

void Component::CreateImGuiComponents(const std::string& aWindowName)
{
	ImGui::Text(("ID: " + std::to_string(myID)).c_str());
	if (ImGui::Checkbox("Active", &myIsActive))
	{
		SetActive(myIsActive);
	}
}

Json::Value Component::ToJson() const
{
	Json::Value result;
	result["IsActive"] = myIsActive;
	result["Type"] = static_cast<int>(myType);
	result["ID"] = myID;
#ifdef _DEBUG
	result["TypeName"] = ComponentTypeToString(myType);
#endif // _DEBUG

	return result;
}

const Component* Component::GetTypePointer() const
{
	return this;
}

void Component::MarkAsPrefabComponent(unsigned anID)
{
	if (myID != anID)
	{
		const_cast<unsigned&>(myID) = anID;
		localIDCount--;
	}	
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
