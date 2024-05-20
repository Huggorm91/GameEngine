#pragma once
#include "ColliderComponent.h"

BEGIN_COMPONENT(CapsuleColliderComponent, ColliderComponent)
public:
	CapsuleColliderComponent();
	CapsuleColliderComponent(const CapsuleColliderComponent& aComponent) = default;
	CapsuleColliderComponent(CapsuleColliderComponent&& aComponent) noexcept = default;
	CapsuleColliderComponent(const Json::Value& aJson);
	~CapsuleColliderComponent() = default;

	CapsuleColliderComponent& operator=(const CapsuleColliderComponent& aComponent) = default;
	CapsuleColliderComponent& operator=(CapsuleColliderComponent&& aComponent) noexcept = default;

	void SetOffset(const Crimson::Vector3f& anOffset);
	const Crimson::Vector3f& GetOffset() const;

	void SetRadius(float aRadius);
	float GetRadius() const;

	void SetHeight(float aHeight);
	float GetHeight() const;

	const Crimson::Vector3f& GetCenter() const;

	bool IsColliding(const ColliderComponent& aCollider) const override;
	bool IsColliding(const BoxColliderComponent& aBox) const override;
	bool IsColliding(const SphereColliderComponent& aSphere) const override;
	bool IsColliding(const RayColliderComponent& aRay) const override;
	bool IsColliding(const CapsuleColliderComponent& aCapsule) const override;

	bool IsValid() const override;

	Json::Value ToJson() const override;

private:
	Crimson::Vector3f myWorldPosition;
	Crimson::Vector3f myOffset;
	float myRadius;
	float myHeight;

	void UpdateWorldPosition();
	void GeneratePhysXShape();
};