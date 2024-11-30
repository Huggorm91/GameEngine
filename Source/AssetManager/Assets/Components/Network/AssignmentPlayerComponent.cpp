#include "AssetManager.pch.h"
#include "AssignmentPlayerComponent.h"

AssignmentPlayerComponent::AssignmentPlayerComponent(): Component()
{
}

AssignmentPlayerComponent::AssignmentPlayerComponent(const Json::Value& aJson) : Component(aJson)
{
}

void AssignmentPlayerComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	// Add SphereCollider with CollisionLayer::Layer::NetworkCulling
}

