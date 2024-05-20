#pragma once
#include "../Component.h"
#include "ColliderForwards.h"
#include "EnumCollisions.h"
#include <bitset>
#include "Math\Vector3.hpp"

BEGIN_COMPONENT_ABSTRACT(ColliderComponent)
public:
	ColliderComponent(ComponentType aComponentType, bool anIsStatic = true);
	ColliderComponent(const ColliderComponent& aComponent);
	ColliderComponent(ColliderComponent&& aComponent) noexcept;
	ColliderComponent(const Json::Value& aJson);
	virtual ~ColliderComponent();

	virtual ColliderComponent& operator=(const ColliderComponent& aComponent);
	virtual ColliderComponent& operator=(ColliderComponent&& aComponent) noexcept;

	void Update() override;

	virtual bool IsColliding(const ColliderComponent& aCollider) const = 0;
	virtual	bool IsColliding(const BoxColliderComponent& aBox) const = 0;
	virtual	bool IsColliding(const SphereColliderComponent& aSphere) const = 0;
	virtual	bool IsColliding(const RayColliderComponent& aRay) const = 0;
	virtual	bool IsColliding(const CapsuleColliderComponent& aCapsule) const = 0;

	// Checks if component is active and has valid size
	virtual bool IsValid() const = 0;

	CollisionLayer::Layer GetLayer() const;
	void SetLayer(CollisionLayer::Layer aLayer);

	void SetCollidingLayer(CollisionLayer::Layer aLayer, bool aShouldCollideWith);
	void SetCollidingLayers(std::bitset<CollisionLayer::Count> aLayerList);
	bool CollidesWithLayer(CollisionLayer::Layer aLayer) const;

	void SetIsTrigger(bool aState);
	bool IsTrigger() const;

	bool IsColliding() const;

	void TransformHasChanged() const override;

	Json::Value ToJson() const override;

	static void SetDebugDrawColor(CollisionLayer::Layer aLayer, const Crimson::Vector3f& aColor);

protected:
	friend class CollisionManager;

	virtual void UpdateWorldPosition() = 0;

	static std::array<Crimson::Vector3f, CollisionLayer::Count> ourColorMap;

	enum eFlags
	{
		eIsStatic,
		eIsTrigger,
		eIsColliding,
		eHasChanged,
		eHasChangedParent,
		eCount
	};

	std::bitset<eCount> myFlags;
private:

	CollisionLayer::Layer myLayer;
	std::bitset<CollisionLayer::Count> myLayersToCollideWith;
};