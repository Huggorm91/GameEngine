#pragma once
#include "ColliderComponent.h"

class BoxSphereBounds;

BEGIN_COMPONENT(BoxColliderComponent, ColliderComponent)
public:
	BoxColliderComponent();
	BoxColliderComponent(const BoxSphereBounds& aBound, bool anIsStatic = true);
	BoxColliderComponent(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null, bool anIsStatic = true);
	BoxColliderComponent(const BoxColliderComponent& aComponent) = default;
	BoxColliderComponent(BoxColliderComponent&& aComponent) noexcept = default;
	BoxColliderComponent(const Json::Value& aJson);
	~BoxColliderComponent() = default;

	BoxColliderComponent& operator=(const BoxColliderComponent& aComponent) = default;
	BoxColliderComponent& operator=(BoxColliderComponent&& aComponent) noexcept = default;
	BoxColliderComponent& operator=(const BoxSphereBounds& aBound) = delete;

	void DebugDraw() override;

	void InitWithMinMax(const Crimson::Vector3f& aMin, const Crimson::Vector3f& aMax, bool anIsStatic = true);
	void InitWithSizeOffset(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null, bool anIsStatic = true);

	void SetOffset(const Crimson::Vector3f& anOffset);
	const Crimson::Vector3f& GetOffset() const;

	void SetSize(const Crimson::Vector3f& aSize);
	Crimson::Vector3f GetSize() const;
	const Crimson::Vector3f& GetExtents() const;

	const Crimson::Vector3f& GetMin() const;
	const Crimson::Vector3f& GetMax() const;
	Crimson::Vector3f GetCenter() const;

	Crimson::Vector3f GetLocalMin() const;
	Crimson::Vector3f GetLocalMax() const;

	bool IsColliding(const ColliderComponent& aCollider) const override;
	bool IsColliding(const BoxColliderComponent& aBox) const override;
	bool IsColliding(const SphereColliderComponent& aSphere) const override;
	bool IsColliding(const RayColliderComponent& aRay) const override;
	bool IsColliding(const CapsuleColliderComponent& aCapsule) const override;

	bool IsInside(const Crimson::Vector3f& aPoint) const;

	bool IsValid() const override;

	Json::Value ToJson() const override;

private:
	Crimson::Vector3f myOffset;
	Crimson::Vector3f myHalfSize;
	Crimson::Vector3f myWorldMin;
	Crimson::Vector3f myWorldMax;

	void UpdateWorldPosition();
};