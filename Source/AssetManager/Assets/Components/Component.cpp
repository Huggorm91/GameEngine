#include "AssetManager.pch.h"
#include "Component.h"
#include "../GameObject.h"

unsigned int Component::ourIDCount = 0;

Component::Component() : myParent(nullptr), myIsActive(true), myType(ComponentType::Unknown), myID(++ourIDCount)
{}

Component::Component(ComponentType aType) : myParent(nullptr), myIsActive(true), myType(aType), myID(++ourIDCount)
{}

Component::Component(const Component& aComponent) : myParent(aComponent.myParent), myIsActive(aComponent.myIsActive), myType(aComponent.myType), myID(++ourIDCount)
{}

Component::Component(Component&& aComponent) noexcept : myParent(aComponent.myParent), myIsActive(aComponent.myIsActive), myType(aComponent.myType), myID(aComponent.myID)
{}

Component::Component(const Json::Value& aJson) : myParent(nullptr), myIsActive(aJson["IsActive"].asBool()), myType(static_cast<ComponentType>(aJson["Type"].asInt())), myID(aJson["ID"].asUInt())
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
	const_cast<unsigned&>(myID) = aComponent.myID;
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

unsigned Component::GetParentID() const
{
	return myParent->myID;
}

unsigned Component::GetComponentID() const
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

void Component::CreateImGuiComponents(const std::string&)
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
	result["TypeName"] = ToString();
#endif // _DEBUG

	return result;
}

void Component::Serialize(std::ostream& aStream) const
{
	Binary::eType type = Binary::Component;
	aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	aStream.write(reinterpret_cast<const char*>(&myType), sizeof(myType));
	aStream.write(reinterpret_cast<const char*>(&myID), sizeof(myID));
	aStream.write(reinterpret_cast<const char*>(&myIsActive), sizeof(myIsActive));
}

void Component::Deserialize(std::istream& aStream)
{
	aStream.read(reinterpret_cast<char*>(const_cast<unsigned*>(&myID)), sizeof(myID));
	aStream.read(reinterpret_cast<char*>(&myIsActive), sizeof(myIsActive));
}

void Component::MarkAsPrefabComponent(unsigned anID)
{
	if (myID != anID)
	{
		const_cast<unsigned&>(myID) = anID;
		ourIDCount--;
	}
}

void Component::CopyID(const Component* aComponent, bool aDecrementIDCount)
{
	if (aDecrementIDCount)
	{
		ourIDCount--;
	}
	const_cast<unsigned&>(myID) = aComponent->myID;
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