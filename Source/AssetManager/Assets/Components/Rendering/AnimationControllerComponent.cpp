#include "AssetManager.pch.h"
#include "AnimationControllerComponent.h"

AnimationControllerComponent::AnimationControllerComponent() :
	AnimatedMeshComponent(ComponentType::AnimationController),
	myAnimationDelta(0.f),
	myAnimationTimer(0.f)
{}

AnimationControllerComponent::AnimationControllerComponent(const AnimatedMeshComponent& aMeshComponent) :
	AnimatedMeshComponent(aMeshComponent),
	myAnimationDelta(0.f),
	myAnimationTimer(0.f)
{
	if (myAnimation)
	{
		myAnimationDelta = myAnimation->GetFrameDelta();
	}
}

AnimationControllerComponent::AnimationControllerComponent(const Json::Value& aJson) :
	AnimatedMeshComponent(aJson)
{}

AnimationControllerComponent& AnimationControllerComponent::operator=(const AnimatedMeshComponent& aComponent)
{
	AnimatedMeshComponent::operator=(aComponent);
	for (auto& animation : myAdditiveAnimations)
	{
		animation->Init(myBoneTransformCache, mySkeleton);
	}
	return *this;
}

void AnimationControllerComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	UpdateNoRender();
	Render();
}

void AnimationControllerComponent::UpdateNoRender()
{
	if (!myIsActive || !myAnimation)
	{
		return;
	}

	if (myAdditiveAnimations.empty())
	{
		myAnimation->Update();
	}
	else
	{
		// Logic to manually step through animations
	}
}

void AnimationControllerComponent::Init(GameObject* aParent)
{
	AnimatedMeshComponent::Init(aParent);
	for (auto& animation : myAdditiveAnimations)
	{
		animation->Init(myBoneTransformCache, mySkeleton);
	}
}

void AnimationControllerComponent::SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	myAdditiveAnimations.clear();
	AnimatedMeshComponent::SetAnimation(anAnimation);
	if (myAnimationDelta <= 0.f && anAnimation)
	{
		myAnimationDelta = anAnimation->GetFrameDelta();
	}
}

unsigned AnimationControllerComponent::AddAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	if (myAdditiveAnimations.empty())
	{
		myAnimation->SetAdditiveAnimation(true);
	}
	const unsigned index = static_cast<unsigned>(myAdditiveAnimations.size());
	auto& animation = myAdditiveAnimations.emplace_back(anAnimation);
	animation->Init(myBoneTransformCache, mySkeleton);
	animation->SetAdditiveAnimation(true);
	return index;
}

void AnimationControllerComponent::RemoveAnimation(unsigned anIndex)
{
	assert(anIndex < myAdditiveAnimations.size());
	myAdditiveAnimations.erase(myAdditiveAnimations.begin() + anIndex);
	if (myAdditiveAnimations.empty())
	{
		myAnimation->SetAdditiveAnimation(false);
	}
}

void AnimationControllerComponent::StartAnimation()
{
	myAnimation->StartAnimation();
	for (auto& animation : myAdditiveAnimations)
	{
		animation->StartAnimation();
	}
}

void AnimationControllerComponent::StopAnimation()
{
	ResetBoneCache();
	AnimatedMeshComponent::StopAnimation();
	for (auto& animation : myAdditiveAnimations)
	{
		animation->StopAnimation();
		animation->SetToFirstFrame();
	}
}

void AnimationControllerComponent::PauseAnimation()
{
	myAnimation->StopAnimation();
	for (auto& animation : myAdditiveAnimations)
	{
		animation->StopAnimation();
	}
}

void AnimationControllerComponent::SetLooping(bool aIsLooping)
{
	myAnimation->SetIsLooping(aIsLooping);
	for (auto& animation : myAdditiveAnimations)
	{
		animation->SetIsLooping(aIsLooping);
	}
}

void AnimationControllerComponent::ToogleLooping()
{
	myAnimation->ToogleLooping();
	for (auto& animation : myAdditiveAnimations)
	{
		animation->ToogleLooping();
	}
}

void AnimationControllerComponent::SetPlayInReverse(bool aShouldPlayInReverse)
{
	myAnimation->SetIsPlayingInReverse(aShouldPlayInReverse);
	for (auto& animation : myAdditiveAnimations)
	{
		animation->SetIsPlayingInReverse(aShouldPlayInReverse);
	}
}

void AnimationControllerComponent::SetTargetFPS(float aFPS)
{
	myAnimationDelta = 1.f / aFPS;
}

void AnimationControllerComponent::UpdateBoneCache()
{
	ResetBoneCache();
	myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache, myAnimationTimer / myAnimationDelta);
	for (auto& animation : myAdditiveAnimations)
	{
		animation->UpdateBoneCache(mySkeleton, myBoneTransformCache, myAnimationTimer / myAnimationDelta);
	}
}

void AnimationControllerComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	MeshComponent::CreateImGuiComponents(aWindowName);
}

Json::Value AnimationControllerComponent::ToJson() const
{
	Json::Value result = MeshComponent::ToJson();
	return result;
}
