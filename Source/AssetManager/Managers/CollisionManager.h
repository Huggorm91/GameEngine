#pragma once
#include "AssetManager/Assets/Components/Collision/ColliderComponent.h"
#include <unordered_map>
#include <unordered_set>

class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	static CollisionManager& Get() { static CollisionManager instance; return instance; }

	void AddCollider(ColliderComponent* aCollider);
	void RemoveCollider(ColliderComponent* aCollider);

	void CheckCollisions();

	void EndFrame();

	void Reset();

private:
	std::unordered_map<eCollisionLayer, std::unordered_set<ColliderComponent*>> myCollisionObjects;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> myCollisions;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> myPreviousCollisions;

	void CollisionEnter(ColliderComponent* aFirst, ColliderComponent* aSecond) const;
	void CollisionStay(ColliderComponent* aFirst, ColliderComponent* aSecond) const;
	void CollisionExit(ColliderComponent* aFirst, ColliderComponent* aSecond) const;


};