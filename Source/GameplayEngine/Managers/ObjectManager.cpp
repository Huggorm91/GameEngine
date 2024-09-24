#include "GameplayEngine.pch.h"
#include "ObjectManager.h"


void ObjectManager::UpdateObjects(bool aShouldRender)
{
	myIsUpdating = true;
	if (aShouldRender)
	{
		for (auto& [id, object] : myGameObjects)
		{
			object->Update();
			object->Render();
		}
	}
	else
	{
		for (auto& [id, object] : myGameObjects)
		{
			object->Update();
		}
	}	
	myIsUpdating = false;
}

void ObjectManager::RenderObjects(bool aDebugDraw)
{
	myIsUpdating = true;
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
	myIsUpdating = false;
}

void ObjectManager::EndOfFrame()
{
	for (auto& id : myObjectsToRemove)
	{
		InternalRemoveObject(id);
	}
	myObjectsToRemove.clear();
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
		pointer = &myTemporaryObjects.emplace(newObject.GetUUID(), std::move(newObject)).first->second;
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
		pointer = &myTemporaryObjects.emplace(newObject.GetUUID(), newObject).first->second;
	}

	return myGameObjects.emplace(pointer->GetUUID(), pointer).first->second;
}

GameObject* ObjectManager::AddGameObject(GameObject&& anObject, bool anIsPersistant)
{
	if (HasGameObject(anObject.GetUUID()))
	{
		return myGameObjects.at(anObject.GetUUID());
	}

	GameObject* pointer = nullptr;
	if (anIsPersistant)
	{
		pointer = &myPersistantObjects.emplace(anObject.GetUUID(), std::move(anObject)).first->second;
	}
	else
	{
		pointer = &myTemporaryObjects.emplace(anObject.GetUUID(), std::move(anObject)).first->second;
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

const std::unordered_map<UUIDv4::UUID, GameObject>& ObjectManager::GetTemporaryObjects()
{
	return myTemporaryObjects;
}

const std::unordered_map<UUIDv4::UUID, GameObject>& ObjectManager::GetPersistantObjects()
{
	return myPersistantObjects;
}

bool ObjectManager::RemoveGameObject(const UUIDv4::UUID& anID)
{
	if (myIsUpdating)
	{
		myObjectsToRemove.emplace_back(anID);
		return true;
	}
	else
	{
		return InternalRemoveObject(anID);
	}
}

void ObjectManager::RemoveGameObjectAtEndOfFrame(const UUIDv4::UUID& anID)
{
	myObjectsToRemove.emplace_back(anID);
}

bool ObjectManager::HasGameObject(const UUIDv4::UUID& anID) const
{
	return myGameObjects.contains(anID);
}

void ObjectManager::SetSceneObjects(std::unordered_map<UUIDv4::UUID, GameObject>* anObjectList)
{
	ClearTemporaryObjects();
	mySceneObjects = anObjectList;
	for (auto& [id, object] : *mySceneObjects)
	{
		myGameObjects.emplace(id, &object);
	}
}

void ObjectManager::MoveTemporaryObjectsToScene()
{
	if (mySceneObjects)
	{
		for (auto& [id, object] : myTemporaryObjects)
		{
			auto iter = mySceneObjects->emplace(id, std::move(object));
			myGameObjects.at(id) = &iter.first->second;
		}
		myTemporaryObjects.clear();
	}
}

void ObjectManager::ClearSceneObjects()
{
	if (mySceneObjects)
	{
		for (auto& [id, object] : *mySceneObjects)
		{
			myGameObjects.erase(id);
		}
		mySceneObjects = nullptr;
	}
}

void ObjectManager::ClearTemporaryObjects()
{
	for (auto& [id, object] : myTemporaryObjects)
	{
		myGameObjects.erase(id);
	}
	myTemporaryObjects.clear();
}

void ObjectManager::ClearPersistantObjects()
{
	for (auto& [id, object] : myPersistantObjects)
	{
		myGameObjects.erase(id);
	}
	myPersistantObjects.clear();
}

bool ObjectManager::InternalRemoveObject(const UUIDv4::UUID& anID)
{
	if (auto iter = myGameObjects.find(anID); iter != myGameObjects.end())
	{
		myGameObjects.erase(iter);
		myPersistantObjects.erase(anID);
		myTemporaryObjects.erase(anID);
		return true;
	}
	return false;
}
