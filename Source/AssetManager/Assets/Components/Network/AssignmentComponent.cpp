#include "AssetManager.pch.h"
#include "AssignmentComponent.h"
#include "GameplayEngine/Managers/ObjectManager.h"
#include "GameplayEngine/Network/NetworkManager.h"

AssignmentComponent::AssignmentComponent() : Component(ComponentType::Assignment)
{
}

AssignmentComponent::AssignmentComponent(const Json::Value& aJson): Component(aJson)
{
}

void AssignmentComponent::OnCollisionEnter(CollisionLayer::Layer , ColliderComponent* )
{
	Engine::GetObjectManager().RemoveGameObjectAtEndOfFrame(GetParentID());
	Engine::GetNetworkManager().SendDeleteGameObject(GetParentID());
}
