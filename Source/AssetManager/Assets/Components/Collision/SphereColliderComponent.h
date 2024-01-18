#pragma once
#include "ColliderComponent.h"

class BoxSphereBounds;

class SphereColliderComponent : public ColliderComponent
{
public:
	SphereColliderComponent();
	SphereColliderComponent(const BoxSphereBounds& aBound);
	SphereColliderComponent(const Crimson::Vector3f& aCenter, float aRadius);
	SphereColliderComponent(const SphereColliderComponent& aComponent) = default;
	SphereColliderComponent(SphereColliderComponent&& aComponent) noexcept = default;
	SphereColliderComponent(const Json::Value& aJson);
	~SphereColliderComponent() = default;

	void InitWithCenterRadius(const Crimson::Vector3f& aCenter, float aRadius);

	void SetOffset(const Crimson::Vector3f& anOffset);
	const Crimson::Vector3f& GetOffset() const;

	void SetRadius(float aRadius);
	float GetRadius() const;
	float GetRadiusSqr() const;

	const Crimson::Vector3f& GetCenter() const;

	SphereColliderComponent& operator=(const SphereColliderComponent& aComponent) = default;
	SphereColliderComponent& operator=(SphereColliderComponent&& aComponent) noexcept = default;
	SphereColliderComponent& operator=(const BoxSphereBounds& aBound);

	bool IsColliding(const ColliderComponent& aCollider) const override;
	bool IsColliding(const BoxColliderComponent& aBox) const override;
	bool IsColliding(const SphereColliderComponent& aSphere) const override;
	bool IsColliding(const RayColliderComponent& aRay) const override;
	bool IsColliding(const CapsuleColliderComponent& aCapsule) const override;

	bool IsInside(const Crimson::Vector3f& aPoint) const;

	bool IsValid() const override;

	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	inline const SphereColliderComponent* GetTypePointer() const override;

private:
	Crimson::Vector3f myWorldPosition;
	Crimson::Vector3f myCenter;
	float myRadius;

	void UpdateWorldPosition();
};