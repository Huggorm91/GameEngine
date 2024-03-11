#include "AssetManager.pch.h"
#include "AnimationControllerComponent.h"

AnimationControllerComponent::AnimationControllerComponent(): 
    MeshComponent(ComponentType::AnimationController), 
    mySkeleton(nullptr)
{}

AnimationControllerComponent::AnimationControllerComponent(const Json::Value& aJson): 
    MeshComponent(aJson),
    mySkeleton(nullptr)
{}

void AnimationControllerComponent::Update()
{}

void AnimationControllerComponent::CreateImGuiComponents(const std::string& aWindowName)
{
    MeshComponent::CreateImGuiComponents(aWindowName);
}

Json::Value AnimationControllerComponent::ToJson() const
{
    Json::Value result = MeshComponent::ToJson();
    return result;
}
