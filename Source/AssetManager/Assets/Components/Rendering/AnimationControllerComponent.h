#pragma once
#include "MeshComponent.h"
#include "..\..\Animation\Animation.h"	
#include "..\..\Animation\Skeleton.h"
#include "..\..\Animation\StateMachine\AnimationStateMachine.h"

BEGIN_COMPONENT(AnimationControllerComponent, MeshComponent)
public:
	AnimationControllerComponent();
	AnimationControllerComponent(const AnimationControllerComponent& aComponent) = default;
	AnimationControllerComponent(AnimationControllerComponent&& aComponent) noexcept = default;
	AnimationControllerComponent(const Json::Value& aJson);
	~AnimationControllerComponent() = default;

	AnimationControllerComponent& operator=(const AnimationControllerComponent& aComponent) = default;
	AnimationControllerComponent& operator=(AnimationControllerComponent&& aComponent) noexcept = default;

	void Update() override;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;

private:
	std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> myBoneTransformCache;
	AnimationStateMachine myStateMachine;
	std::unordered_map<unsigned, std::shared_ptr<AnimationBase>> myAnimations;// List of animations, control what bones are occupied with animations when adding new ones
	Skeleton* mySkeleton;
};