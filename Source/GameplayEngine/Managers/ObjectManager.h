#pragma once
#include "AssetManager/Assets/GameObject.h"

class ObjectManager
{
public:
	ObjectManager() = default;
	~ObjectManager() = default;

	void UpdateObjects();
	void RenderObjects(bool aDebugDraw = false);

	GameObject* AddGameObject(bool anIsPersistant = false);
	GameObject* AddGameObject(const GameObject& anObject, bool anIsPersistant = false);
	GameObject* AddGameObject(GameObject&& anObject, bool anIsPersistant = false);

	GameObject* GetGameObject(const UUIDv4::UUID& anID);
	//GameObject* GetGameObject(const Crimson::Vector2f& aScreenPosition);

	bool RemoveGameObject(const UUIDv4::UUID& anID);

	void SetTemporaryObjects(std::vector<GameObject>* anObjectList);

	void ClearTemporaryObjects();
	void ClearPersistantObjects();

private:
	std::unordered_map<UUIDv4::UUID, GameObject*> myGameObjects;
	std::vector<GameObject> myPersistantObjects;
	std::vector<GameObject>* myTemporaryObjects = nullptr;
};