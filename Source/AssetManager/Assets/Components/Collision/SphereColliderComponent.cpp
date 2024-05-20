#include "AssetManager.pch.h"
#include "SphereColliderComponent.h"
#include "Intersection.h"
#include "Assets\GameObject.h"
#include "../ComponentParts/BoxSphereBounds.h"
#include "GraphicsEngine/Drawer/LineDrawer.h"

SphereColliderComponent::SphereColliderComponent() :
	ColliderComponent(ComponentType::SphereCollider),
	myRadius(0.f)
{
}

SphereColliderComponent::SphereColliderComponent(const BoxSphereBounds& aBound, bool anIsStatic) :
	ColliderComponent(ComponentType::SphereCollider, anIsStatic),
	myOffset(aBound.GetCenter()),
	myRadius(aBound.GetRadius())
{
	myFlags[eHasChanged] = true;
}

SphereColliderComponent::SphereColliderComponent(float aRadius, const Crimson::Vector3f& anOffset, bool anIsStatic) :
	ColliderComponent(ComponentType::SphereCollider, anIsStatic),
	myOffset(anOffset),
	myRadius(aRadius)
{
	myFlags[eHasChanged] = true;
}

SphereColliderComponent::SphereColliderComponent(const Json::Value& aJson) :
	ColliderComponent(aJson),
	myOffset(aJson["Center"]),
	myRadius(aJson["Radius"].asFloat())
{
}

void SphereColliderComponent::DebugDraw()
{
	//globalEngine->debugDrawer->RemovePrimitiveNextFrame(globalEngine->debugDrawer->AddSphere(GetCenter(), GetRadius(), Crimson::Vector3f::Zero, Crimson::Vector4f(ourColorMap[GetLayer()], 1.f)));
}

void SphereColliderComponent::InitWithRadiusOffset(float aRadius, const Crimson::Vector3f& anOffset, bool anIsStatic)
{
	myFlags[eIsStatic] = anIsStatic;
	myOffset = anOffset;
	myRadius = aRadius;
	myFlags[eHasChanged] = true;
}

void SphereColliderComponent::SetOffset(const Crimson::Vector3f& anOffset)
{
	myOffset = anOffset;
	myFlags[eHasChanged] = true;
}

const Crimson::Vector3f& SphereColliderComponent::GetOffset() const
{
	return myOffset;
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
	result["Center"] = static_cast<Json::Value>(myOffset);
	result["Radius"] = myRadius;
	return result; 
}

void SphereColliderComponent::UpdateWorldPosition()
{
	assert(myParent && "SphereColliderComponent is not initialized");
	myWorldPosition = myParent->GetWorldPosition() + myOffset;
	myFlags[eHasChanged] = false;
}
