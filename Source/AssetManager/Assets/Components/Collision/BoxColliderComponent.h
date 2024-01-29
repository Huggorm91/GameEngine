#pragma once
#include "ColliderComponent.h"

class BoxSphereBounds;

class BoxColliderComponent : public ColliderComponent
{
public:
	BoxColliderComponent();
	BoxColliderComponent(const BoxSphereBounds& aBound);
	BoxColliderComponent(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null);
	BoxColliderComponent(const BoxColliderComponent& aComponent) = default;
	BoxColliderComponent(BoxColliderComponent&& aComponent) noexcept = default;
	BoxColliderComponent(const Json::Value& aJson);
	~BoxColliderComponent() = default;

	BoxColliderComponent& operator=(const BoxColliderComponent& aComponent) = default;
	BoxColliderComponent& operator=(BoxColliderComponent&& aComponent) noexcept = default;
	BoxColliderComponent& operator=(const BoxSphereBounds& aBound);

	void InitWithMinMax(const Crimson::Vector3f& aMin, const Crimson::Vector3f& aMax);
	void InitwithSizeOffset(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset = Crimson::Vector3f::Null);

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

	void CreateImGuiComponents(const std::string& aWindowName) override;

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;
	Json::Value ToJson() const override;

	inline std::string ToString() const override;
	inline const BoxColliderComponent* GetTypePointer() const override;

private:
	Crimson::Vector3f myCenter;
	Crimson::Vector3f myHalfSize;
	Crimson::Vector3f myWorldMin;
	Crimson::Vector3f myWorldMax;

	void UpdateWorldPosition();
};