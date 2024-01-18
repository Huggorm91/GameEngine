#include "AssetManager.pch.h"
#include "RayColliderComponent.h"
#include "Intersection.h"
#include "Assets\GameObject.h"

RayColliderComponent::RayColliderComponent() :
	ColliderComponent(ComponentType::RayCollider),
	myLength(0.f)
{
}

RayColliderComponent::RayColliderComponent(const Crimson::Vector3f& aStartPosition, const Crimson::Vector3f& aTargetPosition) :
	ColliderComponent(ComponentType::RayCollider),
	myDirection((aTargetPosition - aStartPosition).GetNormalized()),
	myOrigin(aStartPosition),
	myLength((aTargetPosition - aStartPosition).Length())
{
	myHasChanged = true;
}

RayColliderComponent::RayColliderComponent(const Crimson::Vector3f& anOrigin, const Crimson::Vector3f& aDirection, float aLength) :
	ColliderComponent(ComponentType::RayCollider),
	myDirection(aDirection),
	myOrigin(anOrigin),
	myLength(aLength)
{
	myHasChanged = true;
}

RayColliderComponent::RayColliderComponent(const Json::Value& aJson) :
	ColliderComponent(aJson),
	myDirection(aJson["Direction"]),
	myOrigin(aJson["Origin"]),
	myLength(aJson["Length"].asFloat())
{
}

void RayColliderComponent::InitWith2Points(const Crimson::Vector3f& aStartPosition, const Crimson::Vector3f& aTargetPosition)
{
	auto direction = aTargetPosition - aStartPosition;
	myDirection = direction.GetNormalized();
	myOrigin = aStartPosition;
	myLength = direction.Length();
	myHasChanged = true;
}

void RayColliderComponent::InitWithOriginDirection(const Crimson::Vector3f& anOrigin, const Crimson::Vector3f& aDirection, float aLength)
{
	myDirection = aDirection;
	myOrigin = anOrigin;
	myLength = aLength;
	myHasChanged = true;
}

void RayColliderComponent::SetOffset(const Crimson::Vector3f& anOffset)
{
	myOrigin = anOffset;
	myHasChanged = true;
}

const Crimson::Vector3f& RayColliderComponent::GetOffset() const
{
	return myOrigin;
}

void RayColliderComponent::SetDirection(const Crimson::Vector3f& aDirection)
{
	myDirection = aDirection.GetNormalizedNoAssert();
}

const Crimson::Vector3f& RayColliderComponent::GetDirection() const
{
	return myDirection;
}

void RayColliderComponent::SetLength(float aLength)
{
	myLength = aLength;
}

float RayColliderComponent::GetLength() const
{
	return myLength;
}

const Crimson::Vector3f& RayColliderComponent::GetOrigin() const
{
	return myWorldOrigin;
}

bool RayColliderComponent::IsColliding(const ColliderComponent& aCollider) const
{
	return aCollider.IsColliding(*this);
}

bool RayColliderComponent::IsColliding(const BoxColliderComponent& aBox) const
{
	return Intersection(aBox, *this);
}

bool RayColliderComponent::IsColliding(const SphereColliderComponent& aSphere) const
{
	return Intersection(aSphere, *this);
}

bool RayColliderComponent::IsColliding(const RayColliderComponent& aRay) const
{
	return Intersection(*this, aRay);
}

bool RayColliderComponent::IsColliding(const CapsuleColliderComponent& aCapsule) const
{
	return Intersection(aCapsule, *this);
}

bool RayColliderComponent::IsValid() const
{
	return myIsActive && myLength != 0.f;
}

Json::Value RayColliderComponent::ToJson() const
{
	auto result = ColliderComponent::ToJson();
	result["Direction"] = static_cast<Json::Value>(myDirection);
	result["Origin"] = static_cast<Json::Value>(myOrigin);
	result["Length"] = myLength;
	return result;
}

inline std::string RayColliderComponent::ToString() const
{
	return "RayCollider";
}

inline const RayColliderComponent* RayColliderComponent::GetTypePointer() const
{
	return this;
}

void RayColliderComponent::UpdateWorldPosition()
{
	assert(myParent && "RayColliderComponent is not initialized");
	myWorldOrigin = myParent->GetWorldPosition() + myOrigin;
	myHasChanged = false;
}
