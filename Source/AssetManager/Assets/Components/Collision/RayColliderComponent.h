#pragma once
#include "ColliderComponent.h"

BEGIN_COMPONENT(RayColliderComponent, ColliderComponent)
public:
	RayColliderComponent();
	RayColliderComponent(const Crimson::Vector3f& aStartPosition, const Crimson::Vector3f& aTargetPosition);
	RayColliderComponent(const Crimson::Vector3f& aDirection, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null, float aLength = FLT_MAX);
	RayColliderComponent(const RayColliderComponent& aComponent) = default;
	RayColliderComponent(RayColliderComponent&& aComponent) noexcept = default;
	RayColliderComponent(const Json::Value& aJson);
	~RayColliderComponent() = default;

	RayColliderComponent& operator=(const RayColliderComponent& aComponent) = default;
	RayColliderComponent& operator=(RayColliderComponent&& aComponent) noexcept = default;

	void InitWith2Points(const Crimson::Vector3f& aStartPosition, const Crimson::Vector3f& aTargetPosition);
	void InitWithDirectionOffset(const Crimson::Vector3f& aDirection, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null, float aLength = FLT_MAX);

	void SetOffset(const Crimson::Vector3f& anOffset);
	const Crimson::Vector3f& GetOffset() const;

	void SetDirection(const Crimson::Vector3f& aDirection);
	const Crimson::Vector3f& GetDirection() const;

	// Use INFINITY for infinite lenght
	void SetLength(float aLength);
	float GetLength() const;

	const Crimson::Vector3f& GetOrigin() const;

	bool IsColliding(const ColliderComponent& aCollider) const override;
	bool IsColliding(const BoxColliderComponent& aBox) const override;
	bool IsColliding(const SphereColliderComponent& aSphere) const override;
	bool IsColliding(const RayColliderComponent& aRay) const override;
	bool IsColliding(const CapsuleColliderComponent& aCapsule) const override;

	bool IsValid() const override;

	Json::Value ToJson() const override;

private:
	Crimson::Vector3f myWorldOrigin;
	Crimson::Vector3f myDirection;
	Crimson::Vector3f myOffset;
	float myLength;

	void UpdateWorldPosition();
};