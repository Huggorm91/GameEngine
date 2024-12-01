#pragma once
#include <unordered_map>
#include <unordered_set>

class ColliderComponent;

class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	bool IsColliding(const UUIDv4::UUID& anID);
	bool IsCollidingWith(const UUIDv4::UUID& aFirstID, const UUIDv4::UUID& aSecondID);
	std::unordered_set<UUIDv4::UUID> GetCollidingObjects(const UUIDv4::UUID& anID);

	void AddCollider(ColliderComponent* aCollider);
	void RemoveCollider(ColliderComponent* aCollider);
	void ChangeLayer(ColliderComponent* aCollider, CollisionLayer::Layer aNewLayer);

	void CheckCollisions();
	void EndFrame();
	void Reset();

private:
	std::unordered_map<CollisionLayer::Layer, std::unordered_set<ColliderComponent*>> myCollisionComponents;
	std::unordered_map<UUIDv4::UUID, std::unordered_set<UUIDv4::UUID>> myCollisionObjects;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> myCollisions;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> myPreviousCollisions;
	std::unordered_set<ColliderComponent*> myRemovedColliders;


	void CollisionEnter(ColliderComponent* aFirst, ColliderComponent* aSecond);
	void CollisionStay(ColliderComponent* aFirst, ColliderComponent* aSecond);
	void CollisionExit(ColliderComponent* aFirst, ColliderComponent* aSecond);
};