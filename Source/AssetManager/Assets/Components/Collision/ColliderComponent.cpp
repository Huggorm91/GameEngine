#include "AssetManager.pch.h"
#include "ColliderComponent.h"
#include "Engine\Engine.h"
#include "Managers\CollisionManager.h"
#include "ImGui\imgui.h"

ColliderComponent::ColliderComponent(ComponentType aType):
	Component(aType),
	myIsTrigger(false),
	myIsColliding(false),
	myHasChanged(false),
	myLayer(eCollisionLayer_Default)
{
}

ColliderComponent::ColliderComponent(const Json::Value& aJson) :
	Component(aJson),
	myIsTrigger(aJson["IsTrigger"].asBool()),
	myIsColliding(false),
	myHasChanged(true),
	myLayersToCollideWith(aJson["CollidingLayers"].asUInt64()),
	myLayer(static_cast<eCollisionLayer>(aJson["Layer"].asUInt()))
{
}

ColliderComponent::~ColliderComponent()
{
	Engine::GetCollisionManager().RemoveCollider(this);
}

void ColliderComponent::Update()
{
	if (myHasChanged)
	{
		UpdateWorldPosition();
	}
	if (IsValid())
	{
		Engine::GetCollisionManager().AddCollider(this);
	}
}

Json::Value ColliderComponent::ToJson() const
{
	auto result = Component::ToJson();
	result["IsTrigger"] = myIsTrigger;
	result["CollidingLayers"] = static_cast<unsigned>(myLayersToCollideWith.to_ulong());
	result["Layer"] = myLayer;
	return result;
}

inline std::string ColliderComponent::ToString() const
{
	return "ColliderComponent";
}

inline const ColliderComponent* ColliderComponent::GetTypePointer() const
{
    return this;
}

eCollisionLayer ColliderComponent::GetLayer() const
{
	return myLayer;
}

void ColliderComponent::SetLayer(eCollisionLayer aLayer)
{
	myLayer = aLayer;
}

void ColliderComponent::SetCollidingLayer(eCollisionLayer aLayer, bool aShouldCollideWith)
{
	myLayersToCollideWith[aLayer] = aShouldCollideWith;
}

void ColliderComponent::SetCollidingLayers(std::bitset<eCollisionLayer_Count> aLayerList)
{
	myLayersToCollideWith = aLayerList;
}

bool ColliderComponent::CollidesWithLayer(eCollisionLayer aLayer) const
{
	return myLayersToCollideWith[aLayer];
}

void ColliderComponent::SetIsTrigger(bool aState)
{
	myIsTrigger = aState;
}

bool ColliderComponent::IsTrigger() const
{
	return myIsTrigger;
}

bool ColliderComponent::IsColliding() const
{
	return myIsColliding;
}

void ColliderComponent::TransformHasChanged() const
{
	const_cast<ColliderComponent&>(*this).UpdateWorldPosition();
}
