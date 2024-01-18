#include "AssetManager.pch.h"
#include "CapsuleColliderComponent.h"
#include "Intersection.h"
#include "Assets\GameObject.h"

CapsuleColliderComponent::CapsuleColliderComponent() : 
	ColliderComponent(ComponentType::CapsuleCollider),
	myHeight(0.f),
	myRadius(0.f)
{
}

CapsuleColliderComponent::CapsuleColliderComponent(const Json::Value& aJson) :
	ColliderComponent(aJson),
	myCenter(aJson["Center"]),
	myRadius(aJson["Radius"].asFloat()),
	myHeight(aJson["Height"].asFloat())
{
}

void CapsuleColliderComponent::SetOffset(const Crimson::Vector3f& anOffset)
{
	myCenter = anOffset;
	myHasChanged = true;
}

const Crimson::Vector3f& CapsuleColliderComponent::GetOffset() const
{
	return myCenter;
}

void CapsuleColliderComponent::SetRadius(float aRadius)
{
	myRadius = aRadius;
}

float CapsuleColliderComponent::GetRadius() const
{
	return myRadius;
}

void CapsuleColliderComponent::SetHeight(float aHeight)
{
	myHeight = aHeight;
}

float CapsuleColliderComponent::GetHeight() const
{
	return myHeight;
}

const Crimson::Vector3f& CapsuleColliderComponent::GetCenter() const
{
	return myWorldPosition;
}

bool CapsuleColliderComponent::IsColliding(const ColliderComponent& aCollider) const
{
	return aCollider.IsColliding(*this);
}

bool CapsuleColliderComponent::IsColliding(const BoxColliderComponent& aBox) const
{
	return Intersection(aBox, *this);
}

bool CapsuleColliderComponent::IsColliding(const SphereColliderComponent& aSphere) const
{
	return Intersection(aSphere, *this);
}

bool CapsuleColliderComponent::IsColliding(const RayColliderComponent& aRay) const
{
	return Intersection(*this, aRay);
}

bool CapsuleColliderComponent::IsColliding(const CapsuleColliderComponent& aCapsule) const
{
	return Intersection(*this, aCapsule);
}

bool CapsuleColliderComponent::IsValid() const
{
	return myIsActive && myRadius > 0.f;
}

Json::Value CapsuleColliderComponent::ToJson() const
{
	auto result = ColliderComponent::ToJson();
	result["Center"] = static_cast<Json::Value>(myCenter);
	result["Radius"] = myRadius;
	result["Height"] = myHeight;
	return result;
}

inline std::string CapsuleColliderComponent::ToString() const
{
	return "CapsuleCollider";
}

inline const CapsuleColliderComponent* CapsuleColliderComponent::GetTypePointer() const
{
	return this;
}

void CapsuleColliderComponent::UpdateWorldPosition()
{
	assert(myParent && "CapsuleColliderComponent is not initialized");
	myWorldPosition = myParent->GetWorldPosition() + myCenter;
	myHasChanged = false;
}
