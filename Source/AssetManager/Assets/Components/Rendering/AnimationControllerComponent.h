#pragma once
#include "AnimatedMeshComponent.h"
#include "..\..\Animation\Animation.h"	
#include "..\..\Animation\Skeleton.h"

BEGIN_COMPONENT(AnimationControllerComponent, AnimatedMeshComponent)
public:
	AnimationControllerComponent();
	AnimationControllerComponent(const AnimationControllerComponent& aComponent) = default;
	AnimationControllerComponent(AnimationControllerComponent&& aComponent) noexcept = default;
	AnimationControllerComponent(const Json::Value& aJson);
	~AnimationControllerComponent() = default;

	AnimationControllerComponent& operator=(const AnimationControllerComponent& aComponent) = default;
	AnimationControllerComponent& operator=(AnimationControllerComponent&& aComponent) noexcept = default;

	void Update() override;

	using AnimatedMeshComponent::Init;
	void Init(GameObject* aParent) override;

	void SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation) override;
	unsigned AddAnimation(const std::shared_ptr<AnimationBase>& anAnimation);
	void RemoveAnimation(unsigned anIndex);

	void StartAnimation()override;
	void StopAnimation()override;
	void PauseAnimation()override;

	void SetLooping(bool aIsLooping)override;
	void ToogleLooping()override;

	void SetPlayInReverse(bool aShouldPlayInReverse)override;

	void SetTargetFPS(float aFPS)override;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;

private:
	std::vector<std::shared_ptr<AnimationBase>> myAdditiveAnimations;
	float myAnimationDelta;
};