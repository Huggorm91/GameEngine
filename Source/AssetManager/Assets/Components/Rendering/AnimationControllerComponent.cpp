#include "AssetManager.pch.h"
#include "AnimationControllerComponent.h"

AnimationControllerComponent::AnimationControllerComponent(): Component(ComponentType::AnimationController)
{}

AnimationControllerComponent::AnimationControllerComponent(const Json::Value& aJson): Component(aJson)
{}

void AnimationControllerComponent::CreateImGuiComponents(const std::string& aWindowName)
{}

Json::Value AnimationControllerComponent::ToJson() const
{
    return Json::Value();
}

inline std::string AnimationControllerComponent::ToString() const
{
    return "AnimationController";
}

inline const AnimationControllerComponent* AnimationControllerComponent::GetTypePointer() const
{
    return this;
}
