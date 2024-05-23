#include "AssetManager.pch.h"
#include "ColliderComponent.h"
#include "GameplayEngine/Managers/CollisionManager.h"
#include "ImGui\imgui.h"
#include "Assets\GameObject.h"

std::array<Crimson::Vector3f, CollisionLayer::Count> CreateDefaultColors()
{
	std::array<Crimson::Vector3f, CollisionLayer::Count> result;
	result.fill(Crimson::Vector3f(0.f, 1.f, 0.f));
	return result;
}
std::array<Crimson::Vector3f, CollisionLayer::Count> ColliderComponent::ourColorMap = CreateDefaultColors();

ColliderComponent::ColliderComponent(ComponentType aType, bool anIsStatic) :
	Component(aType),
	myLayer(CollisionLayer::Default)
{
	myFlags[eIsStatic] = anIsStatic;
}

ColliderComponent::ColliderComponent(const ColliderComponent& aComponent) :
	Component(aComponent),
	myFlags(aComponent.myFlags),
	myLayer(aComponent.myLayer),
	myLayersToCollideWith(aComponent.myLayersToCollideWith)
{}

ColliderComponent::ColliderComponent(ColliderComponent&& aComponent) noexcept :
	Component(std::move(aComponent)),
	myFlags(aComponent.myFlags),
	myLayer(aComponent.myLayer),
	myLayersToCollideWith(aComponent.myLayersToCollideWith)
{	
}

ColliderComponent::ColliderComponent(const Json::Value& aJson) :
	Component(aJson),
	myLayersToCollideWith(aJson["CollidingLayers"].asUInt64()),
	myLayer(static_cast<CollisionLayer::Layer>(aJson["Layer"].asUInt()))
{
	myFlags[eIsStatic] = aJson["IsStatic"].isNull() ? true : aJson["IsStatic"].asBool();
	myFlags[eIsTrigger] = aJson["IsTrigger"].asBool();
	myFlags[eHasChanged] = true;
}

ColliderComponent::~ColliderComponent()
{
	CollisionManager::Get().RemoveCollider(this);
}

ColliderComponent& ColliderComponent::operator=(const ColliderComponent& aComponent)
{
	myFlags = aComponent.myFlags;
	myLayersToCollideWith = aComponent.myLayersToCollideWith;
	myLayer = aComponent.myLayer;
	myFlags[eHasChanged] = true;
	return *this;
}

ColliderComponent& ColliderComponent::operator=(ColliderComponent&& aComponent) noexcept
{
	myFlags = aComponent.myFlags;
	myLayersToCollideWith = aComponent.myLayersToCollideWith;
	myLayer = aComponent.myLayer;
	myFlags[eHasChanged] = true;
	return *this;
}

void ColliderComponent::Update()
{
	if (myFlags[eHasChanged])
	{
		if (myFlags[eHasChangedParent])
		{
			const_cast<std::bitset<eCount>&>(myFlags)[eHasChangedParent] = false;
		}
		else
		{
			UpdateWorldPosition();
		}
	}

	if (IsValid())
	{
		CollisionManager::Get().AddCollider(this);
	}
}

Json::Value ColliderComponent::ToJson() const
{
	auto result = Component::ToJson();
	result["IsStatic"] = myFlags[eIsStatic];
	result["IsTrigger"] = myFlags[eIsTrigger];
	result["CollidingLayers"] = static_cast<unsigned>(myLayersToCollideWith.to_ulong());
	result["Layer"] = myLayer;
	return result;
}

void ColliderComponent::SetDebugDrawColor(CollisionLayer::Layer aLayer, const Crimson::Vector3f& aColor)
{
	ourColorMap[aLayer] = aColor;
}

CollisionLayer::Layer ColliderComponent::GetLayer() const
{
	return myLayer;
}

void ColliderComponent::SetLayer(CollisionLayer::Layer aLayer)
{
	myLayer = aLayer;
}

void ColliderComponent::SetCollidingLayer(CollisionLayer::Layer aLayer, bool aShouldCollideWith)
{
	myLayersToCollideWith[aLayer] = aShouldCollideWith;
}

void ColliderComponent::SetCollidingLayers(std::bitset<CollisionLayer::Count> aLayerList)
{
	myLayersToCollideWith = aLayerList;
}

bool ColliderComponent::CollidesWithLayer(CollisionLayer::Layer aLayer) const
{
	return myLayersToCollideWith[aLayer];
}

void ColliderComponent::SetIsTrigger(bool aState)
{
	myFlags[eIsTrigger] = aState;
}

bool ColliderComponent::IsTrigger() const
{
	return myFlags[eIsTrigger];
}

bool ColliderComponent::IsColliding() const
{
	return myFlags[eIsColliding];
}

void ColliderComponent::TransformHasChanged() const
{
	if (myFlags[eHasChangedParent])
	{
		const_cast<std::bitset<eCount>&>(myFlags)[eHasChangedParent] = false;
		return;
	}
	const_cast<ColliderComponent&>(*this).UpdateWorldPosition();
}
