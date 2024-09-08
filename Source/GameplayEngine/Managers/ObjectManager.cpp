#include "GameplayEngine.pch.h"
#include "ObjectManager.h"


void ObjectManager::UpdateObjects()
{
	for (auto& [id, object] : myGameObjects)
	{
		object->Update();
	}
}

void ObjectManager::RenderObjects(bool aDebugDraw)
{
	if (aDebugDraw)
	{
		for (auto& [id, object] : myGameObjects)
		{
			object->Render();
			object->DebugDraw();
		}
	}
	else
	{
		for (auto& [id, object] : myGameObjects)
		{
			object->Render();
		}
	}
}

GameObject* ObjectManager::AddGameObject(bool anIsPersistant)
{
	GameObject* newObject = nullptr;

	if (anIsPersistant)
	{
		newObject = &myPersistantObjects.emplace_back();
	}
	else
	{
		newObject = &myTemporaryObjects->emplace_back();
	}

	return myGameObjects.emplace(newObject->GetUUID(), newObject).first->second;
}

GameObject* ObjectManager::AddGameObject(const GameObject& anObject, bool anIsPersistant)
{
	GameObject* newObject = nullptr;

	if (anIsPersistant)
	{
		newObject = &myPersistantObjects.emplace_back(anObject);
	}
	else
	{
		newObject = &myTemporaryObjects->emplace_back(anObject);
	}

	return myGameObjects.emplace(newObject->GetUUID(), newObject).first->second;
}

GameObject* ObjectManager::AddGameObject(GameObject&& anObject, bool anIsPersistant)
{
	GameObject* newObject = nullptr;

	if (anIsPersistant)
	{
		newObject = &myPersistantObjects.emplace_back(std::move(anObject));
	}
	else
	{
		newObject = &myTemporaryObjects->emplace_back(std::move(anObject));
	}

	return myGameObjects.emplace(newObject->GetUUID(), newObject).first->second;
}

GameObject* ObjectManager::GetGameObject(const UUIDv4::UUID& anID)
{
	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
		return iter->second;
	}
	return nullptr;
}

bool ObjectManager::RemoveGameObject(const UUIDv4::UUID& anID)
{
	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
		myGameObjects.erase(iter);
		return true;
	}
	return false;
}

void ObjectManager::SetTemporaryObjects(std::vector<GameObject>* anObjectList)
{
	ClearTemporaryObjects();
	myTemporaryObjects = anObjectList;
	for (auto& object : *myTemporaryObjects)
	{
		myGameObjects.emplace(object.GetUUID(), &object);
	}
}

void ObjectManager::ClearTemporaryObjects()
{
	if (myTemporaryObjects)
	{
		for (auto& object : *myTemporaryObjects)
		{
			myGameObjects.erase(object.GetUUID());
		}
		myTemporaryObjects = nullptr;
	}
}

void ObjectManager::ClearPersistantObjects()
{
	for (auto& object : myPersistantObjects)
	{
		myGameObjects.erase(object.GetUUID());
	}
	myPersistantObjects.clear();
}
