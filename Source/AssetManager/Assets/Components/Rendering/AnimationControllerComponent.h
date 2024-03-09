#pragma once
#include "../Component.h"

class AnimationControllerComponent : public Component
{
public:
	AnimationControllerComponent(); // Use base constructor: Component(ComponentType::AnimationController)
	AnimationControllerComponent(const AnimationControllerComponent& aComponent) = default;
	AnimationControllerComponent(AnimationControllerComponent&& aComponent) noexcept = default;
	AnimationControllerComponent(const Json::Value& aJson); // Init(const Json::Value& aJson) can be used as an alternative
	~AnimationControllerComponent() = default;

	AnimationControllerComponent& operator=(const AnimationControllerComponent& aComponent) = default;
	AnimationControllerComponent& operator=(AnimationControllerComponent&& aComponent) noexcept = default;

	// void Update() override;

	// void TransformHasChanged() const override;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	inline const AnimationControllerComponent* GetTypePointer() const override;

private:
	// List of animations, controll what bones are occupied with animations when adding new ones
	// Statemachine
};