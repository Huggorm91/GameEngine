#pragma once
#include <unordered_map>
#include <unordered_set>

class ColliderComponent;

class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	void AddCollider(ColliderComponent* aCollider);
	void RemoveCollider(ColliderComponent* aCollider);

	void ChangeLayer(ColliderComponent* aCollider, CollisionLayer::Layer aNewLayer);

	void CheckCollisions();

	void EndFrame();

	void Reset();

private:
	std::unordered_map<CollisionLayer::Layer, std::unordered_set<ColliderComponent*>> myCollisionObjects;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> myCollisions;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> myPreviousCollisions;
	std::unordered_set<ColliderComponent*> myRemovedColliders;


	void CollisionEnter(ColliderComponent* aFirst, ColliderComponent* aSecond) const;
	void CollisionStay(ColliderComponent* aFirst, ColliderComponent* aSecond) const;
	void CollisionExit(ColliderComponent* aFirst, ColliderComponent* aSecond) const;
};