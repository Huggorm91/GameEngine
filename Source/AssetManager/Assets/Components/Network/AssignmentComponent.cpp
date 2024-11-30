#include "AssetManager.pch.h"
#include "AssignmentComponent.h"
#include "GameplayEngine/PostMaster/PostMaster.h"
#include "AssetManager/Assets/GameObject.h"
#include "AssetManager/Assets/Components/Collision/EnumCollisions.h"

AssignmentComponent::AssignmentComponent() : Component(ComponentType::Assignment), myLifeTime(0.f)
{
}

AssignmentComponent::AssignmentComponent(const Crimson::Vector3f& aDirection) : Component(ComponentType::Assignment), myLifeTime(0.f), myDirection(aDirection.GetNormalizedNoAssert())
{
}

AssignmentComponent::AssignmentComponent(const Json::Value& aJson): Component(aJson), myLifeTime(0.f)
{
}

void AssignmentComponent::Update()
{
	float deltaTime = Crimson::Time::GetDeltaTime();
	myLifeTime += deltaTime;
	if (myLifeTime >= 10.f)
	{
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::GameObject_Died, GetParentID() });
		return;
	}

	const auto& newPos = myParent->GetWorldPosition() + myDirection * (200.f * deltaTime);
	myParent->SetPosition(newPos);
}

void AssignmentComponent::OnCollisionEnter(CollisionLayer::Layer aLayer, ColliderComponent*)
{
	if (aLayer == CollisionLayer::Layer::NetworkCulling)
	{
		// Send Enable message to clients
	}
}

void AssignmentComponent::OnCollisionExit(CollisionLayer::Layer aLayer, ColliderComponent*)
{
	if (aLayer == CollisionLayer::Layer::NetworkCulling)
	{
		// Send Disable message to clients
	}
}
