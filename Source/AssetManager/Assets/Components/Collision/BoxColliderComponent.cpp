#include "AssetManager.pch.h"
#include "BoxColliderComponent.h"
#include "Intersection.h"
#include "Assets\GameObject.h"
#include "../ComponentParts/BoxSphereBounds.h"
#include "GraphicsEngine/Drawer/LineDrawer.h"

BoxColliderComponent::BoxColliderComponent() : ColliderComponent(ComponentType::BoxCollider)
{}

BoxColliderComponent::BoxColliderComponent(const BoxSphereBounds& aBound, bool anIsStatic) :
	ColliderComponent(ComponentType::BoxCollider, anIsStatic),
	myOffset(aBound.GetCenter()),
	myHalfSize(aBound.GetBoxExtents())
{
	myFlags[eHasChanged] = true;
}

BoxColliderComponent::BoxColliderComponent(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset, bool anIsStatic) :
	ColliderComponent(ComponentType::BoxCollider, anIsStatic),
	myOffset(anOffset),
	myHalfSize(aSize * 0.5f)
{
	myFlags[eHasChanged] = true;
}

BoxColliderComponent::BoxColliderComponent(const Json::Value& aJson) :
	ColliderComponent(aJson),
	myOffset(aJson["Center"]),
	myHalfSize(Crimson::Vector3f(aJson["Size"]) * 0.5f)
{
}

bool BoxColliderComponent::IsColliding(const ColliderComponent& aCollider) const
{
	return aCollider.IsColliding(*this);
}

bool BoxColliderComponent::IsColliding(const BoxColliderComponent& aBox) const
{
	return Intersection(*this, aBox);
}

bool BoxColliderComponent::IsColliding(const SphereColliderComponent& aSphere) const
{
	return Intersection(*this, aSphere);
}

bool BoxColliderComponent::IsColliding(const RayColliderComponent& aRay) const
{
	return Intersection(*this, aRay);
}

bool BoxColliderComponent::IsColliding(const CapsuleColliderComponent& aCapsule) const
{
	return Intersection(*this, aCapsule);
}

bool BoxColliderComponent::IsInside(const Crimson::Vector3f& aPoint) const
{
	if (myWorldMin.x <= aPoint.x && aPoint.x <= myWorldMax.x &&
		myWorldMin.y <= aPoint.y && aPoint.y <= myWorldMax.y &&
		myWorldMin.z <= aPoint.z && aPoint.z <= myWorldMax.z)
	{
		return true;
	}
	return false;
}

bool BoxColliderComponent::IsValid() const
{
	return myIsActive && myHalfSize != Crimson::Vector3f::Null;
}

Json::Value BoxColliderComponent::ToJson() const
{
	auto result = ColliderComponent::ToJson();
	result["Center"] = static_cast<Json::Value>(myOffset);
	result["Size"] = static_cast<Json::Value>(GetSize());
	return result;
}

void BoxColliderComponent::DebugDraw()
{
	//globalEngine->debugDrawer->RemovePrimitiveNextFrame(globalEngine->debugDrawer->AddBox(GetCenter(), GetSize(), Crimson::Vector3f::Null, Crimson::Vector4f(ourColorMap[GetLayer()], 1.f)));
}

void BoxColliderComponent::InitWithMinMax(const Crimson::Vector3f& aMin, const Crimson::Vector3f& aMax, bool anIsStatic)
{
	myHalfSize = (aMax - aMin) * 0.5f;
	myOffset = aMin + myHalfSize;
	myFlags[eHasChanged] = true;
	myFlags[eIsStatic] = anIsStatic;
}

void BoxColliderComponent::InitWithSizeOffset(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset, bool anIsStatic)
{
	myOffset = anOffset;
	myHalfSize = aSize * 0.5f;
	myFlags[eHasChanged] = true;
	myFlags[eIsStatic] = anIsStatic;
}

void BoxColliderComponent::SetOffset(const Crimson::Vector3f& anOffset)
{
	myOffset = anOffset;
	myFlags[eHasChanged] = true;
}

const Crimson::Vector3f& BoxColliderComponent::GetOffset() const
{
	return myOffset;
}

void BoxColliderComponent::SetSize(const Crimson::Vector3f& aSize)
{
	myHalfSize = aSize * 0.5f;
	myFlags[eHasChanged] = true;
}

Crimson::Vector3f BoxColliderComponent::GetSize() const
{
	return myHalfSize + myHalfSize;
}

const Crimson::Vector3f& BoxColliderComponent::GetExtents() const
{
	return myHalfSize;
}

const Crimson::Vector3f& BoxColliderComponent::GetMin() const
{
	return myWorldMin;
}

const Crimson::Vector3f& BoxColliderComponent::GetMax() const
{
	return myWorldMax;
}

Crimson::Vector3f BoxColliderComponent::GetCenter() const
{
	return (myWorldMin + myWorldMax) * 0.5f;
}

Crimson::Vector3f BoxColliderComponent::GetLocalMin() const
{
	return myOffset - myHalfSize;
}

Crimson::Vector3f BoxColliderComponent::GetLocalMax() const
{
	return myOffset + myHalfSize;
}

void BoxColliderComponent::UpdateWorldPosition()
{
	assert(myParent && "BoxColliderComponent is not initialized");
	auto worldCenter = myParent->GetWorldPosition() + myOffset;
	myWorldMax = worldCenter + myHalfSize;
	myWorldMin = worldCenter - myHalfSize;
	myFlags[eHasChanged] = false;
}