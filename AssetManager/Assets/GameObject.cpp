#include "AssetManager.pch.h"
#include "GameObject.h"

unsigned int GameObject::localIDCount = 0;

GameObject::GameObject() : myComponents(1000), myIndexList(), myCount(0), myTransform(), myIsActive(true), myID(localIDCount++)
{
}

GameObject::GameObject(const GameObject& aGameObject) : myComponents(aGameObject.myComponents), myIndexList(aGameObject.myIndexList), myTransform(aGameObject.myTransform), myCount(aGameObject.myCount), myIsActive(aGameObject.myIsActive), myID(localIDCount++)
{
	for (auto [type, index] : myIndexList)
	{
		myComponents.ChangeValueUnsafe<Component>(index)->Init(this);
	}
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
	return myTransform.GetTransform();
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
