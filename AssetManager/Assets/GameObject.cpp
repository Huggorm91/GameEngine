#include "AssetManager.pch.h"
#include "GameObject.h"
#include "Components/Rendering/AnimatedMeshComponent.h"

unsigned int GameObject::localIDCount = 0;

GameObject::GameObject() : myComponents(1000), myIndexList(), myCount(0), myTransform(), myIsActive(true), myID(localIDCount++)
{
}

GameObject::GameObject(const GameObject& aGameObject) : myComponents(), myIndexList(), myTransform(aGameObject.myTransform), myCount(), myIsActive(aGameObject.myIsActive), myID(localIDCount++)
{
	const Component* pointer = nullptr;
	for (auto [type, index] : aGameObject.myIndexList)
	{
		pointer = aGameObject.myComponents.GetValueUnsafe<Component>(index)->GetTypePointer();
		::AddComponent(pointer, *this);
	}
}

GameObject::GameObject(GameObject&& aGameObject) noexcept : myComponents(aGameObject.myComponents), myIndexList(aGameObject.myIndexList), myTransform(aGameObject.myTransform), myCount(aGameObject.myCount), myIsActive(aGameObject.myIsActive), myID(aGameObject.myID)
{
	for (auto [type, index] : myIndexList)
	{
		myComponents.ChangeValueUnsafe<Component>(index)->Init(this);
#ifdef _DEBUG
		myDebugPointers.emplace_back(myComponents.GetValueUnsafe<Component>(index));
#endif // _DEBUG
	}
}

GameObject::GameObject(const Json::Value& aJson) : myComponents(1000), myIndexList(), myCount(0), myTransform(aJson["Transform"]), myIsActive(aJson["IsActive"].asBool()), myID(aJson["ID"].asUInt())
{
	for (auto& jsonComponent : aJson["Components"])
	{
		 LoadComponent(jsonComponent, *this);
	}
}

GameObject& GameObject::operator=(const GameObject& aGameObject)
{
	myCount = 0;
	myComponents.Clear();
	myIndexList.clear();
	myTransform = aGameObject.myTransform;
	myIsActive = aGameObject.myIsActive;

	const Component* pointer = nullptr;
	for (auto [type, index] : aGameObject.myIndexList)
	{
		pointer = aGameObject.myComponents.GetValueUnsafe<Component>(index)->GetTypePointer();
		::AddComponent(pointer, *this);
	}
	return *this;
}

GameObject& GameObject::operator=(GameObject&& aGameObject)
{
	myCount = aGameObject.myCount;
	myComponents = aGameObject.myComponents;
	myIndexList = aGameObject.myIndexList;
	myTransform = aGameObject.myTransform;
	myIsActive = aGameObject.myIsActive;

	for (auto [type, index] : myIndexList)
	{
		myComponents.ChangeValueUnsafe<Component>(index)->Init(this);
#ifdef _DEBUG
		myDebugPointers.emplace_back(myComponents.GetValueUnsafe<Component>(index));
#endif // _DEBUG
	}
	return *this;
}

void GameObject::Update()
{
	if (myIsActive)
	{
		for (auto [type, index] : myIndexList)
		{
			myComponents.ChangeValueUnsafe<Component>(index)->Update();
		}
	}
}

void GameObject::SetPosition(const CommonUtilities::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void GameObject::SetRotation(const CommonUtilities::Vector3f& aRotation)
{
	myTransform.SetRotation(aRotation);
}

void GameObject::SetScale(const CommonUtilities::Vector3f& aScale)
{
	myTransform.SetScale(aScale);
}

const Transform& GameObject::GetTransform() const
{
	return myTransform;
}

const CommonUtilities::Matrix4x4f& GameObject::GetTransformMatrix() const
{
	if (myTransform.HasChanged())
	{
		if (HasComponent<MeshComponent>())
		{
			auto componentList = GetComponents<MeshComponent>();
			for (auto& component : componentList)
			{
				component->BoundsHasChanged();
			}
		}
		else if (HasComponent<AnimatedMeshComponent>())
		{
			auto componentList = GetComponents<AnimatedMeshComponent>();
			for (auto& component : componentList)
			{
				component->BoundsHasChanged();
			}
		}		
	}
	return myTransform.GetTransformMatrix();
}

const CommonUtilities::Vector3f& GameObject::GetWorldPosition() const
{
	return myTransform.GetWorldPosition();
}

void GameObject::SetActive(bool aIsActive)
{
	myIsActive = aIsActive;
}

void GameObject::ToogleActive()
{
	myIsActive = !myIsActive;
}

bool GameObject::IsActive() const
{
	return myIsActive;
}

unsigned int GameObject::GetComponentCount() const
{
	return myCount;
}

unsigned int GameObject::GetID() const
{
	return myID;
}

Json::Value GameObject::ToJson() const
{
	Json::Value result;
	Json::String comment("// ID: " + std::to_string(myID));
	result["Components"].setComment(comment, Json::CommentPlacement::commentBefore);

	result["IsActive"] = myIsActive;
	result["ID"] = myID;
	result["Transform"] = myTransform.ToJson();
	result["Components"] = Json::arrayValue;

	const Component* component = nullptr;
	unsigned i = 0;
	for (auto [type, index] : myIndexList)
	{		
		component = myComponents.GetValueUnsafe<Component>(index);
		result["Components"][i] = component->ToJson();
		result["Components"][i].setComment("// " + ComponentTypeToString(component->GetType()), Json::commentBefore);
		++i;
	}

	return result;
}

void GameObject::ToBinary(std::ofstream& aStream) const
{
	//aStream.write();
}