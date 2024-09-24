#pragma once
#include "AssetManager/Assets/GameObject.h"

class ObjectManager
{
public:
	ObjectManager() = default;
	~ObjectManager() = default;

	void UpdateObjects(bool aShouldRender = true);
	void RenderObjects(bool aDebugDraw = false);

	void EndOfFrame();

	GameObject* AddGameObject(bool anIsPersistant = false);
	GameObject* AddGameObject(const GameObject& anObject, bool anIsPersistant = false);
	GameObject* AddGameObject(GameObject&& anObject, bool anIsPersistant = false);

	GameObject* GetGameObject(const UUIDv4::UUID& anID);
	//GameObject* GetGameObject(const Crimson::Vector2f& aScreenPosition);

	const std::unordered_map<UUIDv4::UUID, GameObject>& GetTemporaryObjects();
	const std::unordered_map<UUIDv4::UUID, GameObject>& GetPersistantObjects();

	bool RemoveGameObject(const UUIDv4::UUID& anID);
	void RemoveGameObjectAtEndOfFrame(const UUIDv4::UUID& anID);

	bool HasGameObject(const UUIDv4::UUID& anID) const;

	void SetSceneObjects(std::unordered_map<UUIDv4::UUID, GameObject>* anObjectList);

	void MoveTemporaryObjectsToScene();

	void ClearSceneObjects();
	void ClearTemporaryObjects();
	void ClearPersistantObjects();

private:
	std::unordered_map<UUIDv4::UUID, GameObject*> myGameObjects;
	std::unordered_map<UUIDv4::UUID, GameObject> myPersistantObjects;
	std::unordered_map<UUIDv4::UUID, GameObject> myTemporaryObjects;
	std::unordered_map<UUIDv4::UUID, GameObject>* mySceneObjects = nullptr;
	std::vector<UUIDv4::UUID> myObjectsToRemove;
	bool myIsUpdating = false;

	bool InternalRemoveObject(const UUIDv4::UUID& anID);
};