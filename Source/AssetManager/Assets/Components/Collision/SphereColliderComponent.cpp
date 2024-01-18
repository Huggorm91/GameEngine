#include "AssetManager.pch.h"
#include "SphereColliderComponent.h"
#include "Intersection.h"
#include "Assets\GameObject.h"
#include "../ComponentParts/BoxSphereBounds.h"

SphereColliderComponent::SphereColliderComponent() :
	ColliderComponent(ComponentType::SphereCollider),
	myRadius(0.f)
{
}

SphereColliderComponent::SphereColliderComponent(const BoxSphereBounds& aBound) :
	ColliderComponent(ComponentType::SphereCollider),
	myCenter(aBound.GetCenter()),
	myRadius(aBound.GetRadius())
{
	myHasChanged = true;
}

SphereColliderComponent::SphereColliderComponent(const Crimson::Vector3f& aCenter, float aRadius) :
	ColliderComponent(ComponentType::SphereCollider),
	myCenter(aCenter),
	myRadius(aRadius)
{
	myHasChanged = true;
}

SphereColliderComponent::SphereColliderComponent(const Json::Value& aJson) :
	ColliderComponent(aJson),
	myCenter(aJson["Center"]),
	myRadius(aJson["Radius"].asFloat())
{
}

void SphereColliderComponent::InitWithCenterRadius(const Crimson::Vector3f& aCenter, float aRadius)
{
	myCenter = aCenter;
	myRadius = aRadius;
	myHasChanged = true;
}

void SphereColliderComponent::SetOffset(const Crimson::Vector3f& anOffset)
{
	myCenter = anOffset;
	myHasChanged = true;
}

const Crimson::Vector3f& SphereColliderComponent::GetOffset() const
{
	return myCenter;
}

void SphereColliderComponent::SetRadius(float aRadius)
{
	myRadius = aRadius;
}

float SphereColliderComponent::GetRadius() const
{
	return myRadius;
}

float SphereColliderComponent::GetRadiusSqr() const
{
	return myRadius * myRadius;
}

const Crimson::Vector3f& SphereColliderComponent::GetCenter() const
{
	return myWorldPosition;
}

SphereColliderComponent& SphereColliderComponent::operator=(const BoxSphereBounds& aBound)
{
	myCenter = aBound.GetCenter();
	myRadius = aBound.GetRadius();
	myHasChanged = true;
	return *this;
}

bool SphereColliderComponent::IsColliding(const ColliderComponent& aCollider) const
{
	return aCollider.IsColliding(*this);
}

bool SphereColliderComponent::IsColliding(const BoxColliderComponent& aBox) const
{
	return Intersection(aBox, *this);
}

bool SphereColliderComponent::IsColliding(const SphereColliderComponent& aSphere) const
{
	return Intersection(*this, aSphere);
}

bool SphereColliderComponent::IsColliding(const RayColliderComponent& aRay) const
{
	return Intersection(*this, aRay);
}

bool SphereColliderComponent::IsColliding(const CapsuleColliderComponent& aCapsule) const
{
	return Intersection(*this, aCapsule);
}

bool SphereColliderComponent::IsInside(const Crimson::Vector3f& aPoint) const
{
	Crimson::Vector3f distance(aPoint - GetCenter());
	if (distance.LengthSqr() <= (myRadius * myRadius))
	{
		return true;
	}
	return false;
}

bool SphereColliderComponent::IsValid() const
{
	return myIsActive && myRadius > 0.f;
}

Json::Value SphereColliderComponent::ToJson() const
{
	auto result = ColliderComponent::ToJson();
	result["Center"] = static_cast<Json::Value>(myCenter);
	result["Radius"] = myRadius;
	return result; 
}

inline std::string SphereColliderComponent::ToString() const
{
	return "SphereCollider";
}

inline const SphereColliderComponent* SphereColliderComponent::GetTypePointer() const
{
	return this;
}

void SphereColliderComponent::UpdateWorldPosition()
{
	assert(myParent && "SphereColliderComponent is not initialized");
	myWorldPosition = myParent->GetWorldPosition() + myCenter;
	myHasChanged = false;
}