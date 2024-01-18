#pragma once
#include "ColliderComponent.h"

class RayColliderComponent : public ColliderComponent
{
public:
	RayColliderComponent();
	RayColliderComponent(const Crimson::Vector3f& aStartPosition, const Crimson::Vector3f& aTargetPosition);
	RayColliderComponent(const Crimson::Vector3f& anOrigin, const Crimson::Vector3f& aDirection, float aLength);
	RayColliderComponent(const RayColliderComponent& aComponent) = default;
	RayColliderComponent(RayColliderComponent&& aComponent) noexcept = default;
	RayColliderComponent(const Json::Value& aJson);
	~RayColliderComponent() = default;

	RayColliderComponent& operator=(const RayColliderComponent& aComponent) = default;
	RayColliderComponent& operator=(RayColliderComponent&& aComponent) noexcept = default;

	void InitWith2Points(const Crimson::Vector3f& aStartPosition, const Crimson::Vector3f& aTargetPosition);
	void InitWithOriginDirection(const Crimson::Vector3f& anOrigin, const Crimson::Vector3f& aDirection, float aLength = INFINITY);

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
	inline std::string ToString() const override;
	inline const RayColliderComponent* GetTypePointer() const override;

private:
	Crimson::Vector3f myWorldOrigin;
	Crimson::Vector3f myDirection;
	Crimson::Vector3f myOrigin;
	float myLength;

	void UpdateWorldPosition();
};