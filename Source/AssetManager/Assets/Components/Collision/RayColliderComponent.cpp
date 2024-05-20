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
	myOffset(aStartPosition),
	myLength((aTargetPosition - aStartPosition).Length())
{
	myFlags[eHasChanged] = true;
}

RayColliderComponent::RayColliderComponent(const Crimson::Vector3f& aDirection, const Crimson::Vector3f& anOffset, float aLength) :
	ColliderComponent(ComponentType::RayCollider),
	myDirection(aDirection),
	myOffset(anOffset),
	myLength(Crimson::Abs(aLength))
{
	myFlags[eHasChanged] = true;
}

RayColliderComponent::RayColliderComponent(const Json::Value& aJson) :
	ColliderComponent(aJson),
	myDirection(aJson["Direction"]),
	myOffset(aJson["Origin"]),
	myLength(aJson["Length"].asFloat())
{
}

void RayColliderComponent::InitWith2Points(const Crimson::Vector3f& aStartPosition, const Crimson::Vector3f& aTargetPosition)
{
	auto direction = aTargetPosition - aStartPosition;
	myDirection = direction.GetNormalized();
	myOffset = aStartPosition;
	myLength = direction.Length();
	myFlags[eHasChanged] = true;
}

void RayColliderComponent::InitWithDirectionOffset(const Crimson::Vector3f& aDirection, const Crimson::Vector3f& anOffset, float aLength)
{
	myDirection = aDirection;
	myOffset = anOffset;
	myLength = Crimson::Abs(aLength);
	myFlags[eHasChanged] = true;
}

void RayColliderComponent::SetOffset(const Crimson::Vector3f& anOffset)
{
	myOffset = anOffset;
	myFlags[eHasChanged] = true;
}

const Crimson::Vector3f& RayColliderComponent::GetOffset() const
{
	return myOffset;
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
	myLength = Crimson::Abs(aLength);
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
	result["Origin"] = static_cast<Json::Value>(myOffset);
	result["Length"] = myLength;
	return result;
}

void RayColliderComponent::UpdateWorldPosition()
{
	assert(myParent && "RayColliderComponent is not initialized");
	myWorldOrigin = myParent->GetWorldPosition() + myOffset;
	myFlags[eHasChanged] = false;
}
