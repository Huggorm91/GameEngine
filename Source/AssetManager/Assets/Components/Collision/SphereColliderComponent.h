#pragma once
#include "ColliderComponent.h"

class BoxSphereBounds;

BEGIN_COMPONENT(SphereColliderComponent, ColliderComponent)
public:
	SphereColliderComponent();
	SphereColliderComponent(const BoxSphereBounds& aBound, bool anIsStatic = true);
	SphereColliderComponent(float aRadius, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null, bool anIsStatic = true);
	SphereColliderComponent(const SphereColliderComponent& aComponent) = default;
	SphereColliderComponent(SphereColliderComponent&& aComponent) noexcept = default;
	SphereColliderComponent(const Json::Value& aJson);
	~SphereColliderComponent() = default;

	void DebugDraw() override;

	void InitWithRadiusOffset(float aRadius, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null, bool anIsStatic = true);

	void SetOffset(const Crimson::Vector3f& anOffset);
	const Crimson::Vector3f& GetOffset() const;

	void SetRadius(float aRadius);
	float GetRadius() const;
	float GetRadiusSqr() const;

	const Crimson::Vector3f& GetCenter() const;

	SphereColliderComponent& operator=(const SphereColliderComponent& aComponent) = default;
	SphereColliderComponent& operator=(SphereColliderComponent&& aComponent) noexcept = default;
	SphereColliderComponent& operator=(const BoxSphereBounds& aBound) = delete;

	bool IsColliding(const ColliderComponent& aCollider) const override;
	bool IsColliding(const BoxColliderComponent& aBox) const override;
	bool IsColliding(const SphereColliderComponent& aSphere) const override;
	bool IsColliding(const RayColliderComponent& aRay) const override;
	bool IsColliding(const CapsuleColliderComponent& aCapsule) const override;

	bool IsInside(const Crimson::Vector3f& aPoint) const;

	bool IsValid() const override;

	Json::Value ToJson() const override;

private:
	Crimson::Vector3f myWorldPosition;
	Crimson::Vector3f myOffset;
	float myRadius;

	void UpdateWorldPosition();
};