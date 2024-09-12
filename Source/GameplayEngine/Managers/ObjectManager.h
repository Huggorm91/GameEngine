#pragma once
#include "AssetManager/Assets/GameObject.h"

class ObjectManager
{
public:
	ObjectManager() = default;
	~ObjectManager() = default;

	void UpdateObjects(bool aShouldRender = true);
	void RenderObjects(bool aDebugDraw = false);

	GameObject* AddGameObject(bool anIsPersistant = false);
	GameObject* AddGameObject(const GameObject& anObject, bool anIsPersistant = false);
	GameObject* AddGameObject(GameObject&& anObject, bool anIsPersistant = false);

	GameObject* GetGameObject(const UUIDv4::UUID& anID);
	//GameObject* GetGameObject(const Crimson::Vector2f& aScreenPosition);

	bool RemoveGameObject(const UUIDv4::UUID& anID);

	void SetTemporaryObjects(std::unordered_map<UUIDv4::UUID, GameObject>* anObjectList);

	void ClearTemporaryObjects();
	void ClearPersistantObjects();

private:
	std::unordered_map<UUIDv4::UUID, GameObject*> myGameObjects;
	std::unordered_map<UUIDv4::UUID, GameObject> myPersistantObjects;
	std::unordered_map<UUIDv4::UUID, GameObject>* myTemporaryObjects = nullptr;
};