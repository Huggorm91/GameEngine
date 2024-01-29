#pragma once
#include "../Component.h"
#include "ColliderForwards.h"
#include "EnumCollisions.h"
#include <bitset>

class ColliderComponent : public Component
{
public:
	ColliderComponent(ComponentType aComponentType);
	ColliderComponent(const ColliderComponent& aComponent) = default;
	ColliderComponent(ColliderComponent&& aComponent) noexcept = default;
	ColliderComponent(const Json::Value& aJson);
	virtual ~ColliderComponent();

	virtual ColliderComponent& operator=(const ColliderComponent& aComponent) = default;
	virtual ColliderComponent& operator=(ColliderComponent&& aComponent) noexcept = default;

	void Update() override;

	virtual bool IsColliding(const ColliderComponent& aCollider) const = 0;
	virtual	bool IsColliding(const BoxColliderComponent& aBox) const = 0;
	virtual	bool IsColliding(const SphereColliderComponent& aSphere) const = 0;
	virtual	bool IsColliding(const RayColliderComponent& aRay) const = 0;
	virtual	bool IsColliding(const CapsuleColliderComponent& aCapsule) const = 0;

	// Checks if component is active and has valid size
	virtual bool IsValid() const = 0;

	eCollisionLayer GetLayer() const;
	void SetLayer(eCollisionLayer aLayer);

	void SetCollidingLayer(eCollisionLayer aLayer, bool aShouldCollideWith);
	void SetCollidingLayers(std::bitset<eCollisionLayer_Count> aLayerList);
	bool CollidesWithLayer(eCollisionLayer aLayer) const;

	void SetIsTrigger(bool aState);
	bool IsTrigger() const;

	bool IsColliding() const;

	void TransformHasChanged() const override;

	void CreateImGuiComponents(const std::string& aWindowName) override;

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;
	Json::Value ToJson() const override;

	inline std::string ToString() const override;
	inline const ColliderComponent* GetTypePointer() const override;

protected:
	bool myHasChanged;

	virtual void UpdateWorldPosition() = 0;

private:
	friend class CollisionManager;

	bool myIsTrigger;
	bool myIsColliding;

	std::bitset<eCollisionLayer_Count> myLayersToCollideWith;
	eCollisionLayer myLayer;
};