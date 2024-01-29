#include "AssetManager.pch.h"
#include "BoxColliderComponent.h"
#include "Intersection.h"
#include "Assets\GameObject.h"
#include "../ComponentParts/BoxSphereBounds.h"

BoxColliderComponent::BoxColliderComponent() : ColliderComponent(ComponentType::BoxCollider)
{
}

BoxColliderComponent::BoxColliderComponent(const BoxSphereBounds& aBound) :
	ColliderComponent(ComponentType::BoxCollider),
	myCenter(aBound.GetCenter()),
	myHalfSize(aBound.GetBoxExtents())
{
	myHasChanged = true;
}

BoxColliderComponent::BoxColliderComponent(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset) :
	ColliderComponent(ComponentType::BoxCollider),
	myCenter(anOffset),
	myHalfSize(aSize * 0.5f)
{
	myHasChanged = true;
}

BoxColliderComponent::BoxColliderComponent(const Json::Value& aJson) :
	ColliderComponent(aJson),
	myCenter(aJson["Center"]),
	myHalfSize(Crimson::Vector3f(aJson["Size"]) * 0.5f)
{
}

BoxColliderComponent& BoxColliderComponent::operator=(const BoxSphereBounds& aBound)
{
	myCenter = aBound.GetCenter();
	myHalfSize = aBound.GetBoxExtents();
	myHasChanged = true;
	return *this;
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

void BoxColliderComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	ColliderComponent::CreateImGuiComponents(aWindowName);

	if (ImGui::DragFloat3("Offset", &myCenter.x))
	{
		UpdateWorldPosition();
	}

	Crimson::Vector3f size = GetSize();
	if (ImGui::DragFloat3("Size", &size.x))
	{
		SetSize(size);
		UpdateWorldPosition();
	}
}

void BoxColliderComponent::Serialize(std::ostream& aStream) const
{
	ColliderComponent::Serialize(aStream);
	myCenter.Serialize(aStream);
	myHalfSize.Serialize(aStream);
}

void BoxColliderComponent::Deserialize(std::istream& aStream)
{
	ColliderComponent::Deserialize(aStream);
	myCenter.Deserialize(aStream);
	myHalfSize.Deserialize(aStream);
	myHasChanged = true;
}

Json::Value BoxColliderComponent::ToJson() const
{
	auto result = ColliderComponent::ToJson();
	result["Center"] = static_cast<Json::Value>(myCenter);
	result["Size"] = static_cast<Json::Value>(GetSize());
	return result;
}

inline std::string BoxColliderComponent::ToString() const
{
	return "BoxCollider";
}

inline const BoxColliderComponent* BoxColliderComponent::GetTypePointer() const
{
	return this;
}

void BoxColliderComponent::UpdateWorldPosition()
{
	assert(myParent && "BoxColliderComponent is not initialized");
	auto worldCenter = myParent->GetWorldPosition() + myCenter;
	myWorldMax = worldCenter + myHalfSize;
	myWorldMin = worldCenter - myHalfSize;
	myHasChanged = false;
}

void BoxColliderComponent::InitWithMinMax(const Crimson::Vector3f& aMin, const Crimson::Vector3f& aMax)
{
	myHalfSize = (aMax - aMin) * 0.5f;
	myCenter = aMin + myHalfSize;
	myHasChanged = true;
}

void BoxColliderComponent::InitwithSizeOffset(const Crimson::Vector3f& aSize, const Crimson::Vector3f& anOffset)
{
	myCenter = anOffset;
	myHalfSize = aSize * 0.5f;
	myHasChanged = true;
}

void BoxColliderComponent::SetOffset(const Crimson::Vector3f& anOffset)
{
	myCenter = anOffset;
	myHasChanged = true;
}

const Crimson::Vector3f& BoxColliderComponent::GetOffset() const
{
	return myCenter;
}

void BoxColliderComponent::SetSize(const Crimson::Vector3f& aSize)
{
	myHalfSize = aSize * 0.5f;
	myHasChanged = true;
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
	return myCenter - myHalfSize;
}

Crimson::Vector3f BoxColliderComponent::GetLocalMax() const
{
	return myCenter + myHalfSize;
}
