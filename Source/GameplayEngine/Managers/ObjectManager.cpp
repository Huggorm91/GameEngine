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
	GameObject* pointer = nullptr;
	GameObject newObject;
	if (anIsPersistant)
	{
		pointer = &myPersistantObjects.emplace(newObject.GetUUID(), std::move(newObject)).first->second;
	}
	else
	{
		pointer = &myTemporaryObjects->emplace(newObject.GetUUID(), std::move(newObject)).first->second;
	}

	return myGameObjects.emplace(pointer->GetUUID(), pointer).first->second;
}

GameObject* ObjectManager::AddGameObject(const GameObject& anObject, bool anIsPersistant)
{
	GameObject* pointer = nullptr;
	GameObject newObject = anObject;
	if (anIsPersistant)
	{
		pointer = &myPersistantObjects.emplace(newObject.GetUUID(), newObject).first->second;
	}
	else
	{
		pointer = &myTemporaryObjects->emplace(newObject.GetUUID(), newObject).first->second;
	}

	return myGameObjects.emplace(pointer->GetUUID(), pointer).first->second;
}

GameObject* ObjectManager::AddGameObject(GameObject&& anObject, bool anIsPersistant)
{
	GameObject* pointer = nullptr;
	if (anIsPersistant)
	{
		pointer = &myPersistantObjects.emplace(anObject.GetUUID(), std::move(anObject)).first->second;
	}
	else
	{
		pointer = &myTemporaryObjects->emplace(anObject.GetUUID(), std::move(anObject)).first->second;
	}

	return myGameObjects.emplace(pointer->GetUUID(), pointer).first->second;
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
		// TODO: Fix "memory leak" casued by never removing objects from containers
		return true;
	}
	return false;
}

void ObjectManager::SetTemporaryObjects(std::unordered_map<UUIDv4::UUID, GameObject>* anObjectList)
{
	ClearTemporaryObjects();
	myTemporaryObjects = anObjectList;
	for (auto& [id, object] : *myTemporaryObjects)
	{
		myGameObjects.emplace(id, &object);
	}
}

void ObjectManager::ClearTemporaryObjects()
{
	if (myTemporaryObjects)
	{
		for (auto& [id, object] : *myTemporaryObjects)
		{
			myGameObjects.erase(id);
		}
		myTemporaryObjects = nullptr;
	}
}

void ObjectManager::ClearPersistantObjects()
{
	for (auto& [id, object] : myPersistantObjects)
	{
		myGameObjects.erase(id);
	}
	myPersistantObjects.clear();
}
