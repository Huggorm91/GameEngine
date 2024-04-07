#include "AssetManager.pch.h"
#include "AnimationControllerComponent.h"
#include "Time/Timer.h"

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

	myAnimation->Update();

	if (!myAdditiveAnimations.empty())
	{
		for (auto& animation : myAdditiveAnimations)
		{
			animation->Update();
		}

		myAnimationTimer += Crimson::Timer::GetDeltaTime();
		if (myAnimationTimer >= myAnimationDelta)
		{
			myAnimationTimer = 0.f;

			auto resultTransforms = myAnimation->GetAdditiveTransforms();
			for (auto& animation : myAdditiveAnimations)
			{
				auto currentTransforms = animation->GetAdditiveTransforms();

				if (resultTransforms.size() < currentTransforms.size())
				{
					for (auto& [boneName, transform] : currentTransforms)
					{
						if (auto iter = resultTransforms.find(boneName); iter != resultTransforms.end())
						{
							iter->second = AnimationTransform::Interpolate(iter->second, transform, 0.5f);
						}
						else
						{
							resultTransforms.emplace(boneName, transform);
						}
					}
				}
				else
				{
					for (auto& [boneName, transform] : resultTransforms)
					{
						if (auto iter = currentTransforms.find(boneName); iter != currentTransforms.end())
						{
							transform = AnimationTransform::Interpolate(iter->second, transform, 0.5f);
						}
					}
				}
			}

			unsigned index = 0;
			if (myAnimation->IsUsingNamespace()) // If and else should be identical apart from using bone.name vs bone.namespaceName
			{
				for (auto& bone : mySkeleton->GetBones())
				{
					if (auto iter = resultTransforms.find(bone.namespaceName); iter != resultTransforms.end())
					{
						myBoneTransformCache[index] = bone.bindPoseInverse * iter->second.GetAsMatrix();
					}
					else
					{
						myBoneTransformCache[index] = bone.bindPoseInverse;
					}
					++index;
				}
			}
			else
			{
				for (auto& bone : mySkeleton->GetBones())
				{
					if (auto iter = resultTransforms.find(bone.namespaceName); iter != resultTransforms.end())
					{
						myBoneTransformCache[index] = bone.bindPoseInverse * iter->second.GetAsMatrix();
					}
					else
					{
						myBoneTransformCache[index] = bone.bindPoseInverse;
					}
					++index;
				}
			}
		}
	}
	myAnimation->UpdateRootMotion(Crimson::Timer::GetDeltaTime());
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
	if (anAnimation)
	{
		if (myAnimationDelta <= 0.f)
		{
			myAnimationDelta = anAnimation->GetFrameDelta();
		}
		else
		{
			myAnimation->SetTargetFrameDelta(myAnimationDelta);
		}		
	}
}

size_t AnimationControllerComponent::AddAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	if (myAdditiveAnimations.empty())
	{
		myAnimation->SetAdditiveAnimation(true);
	}
	const size_t index = myAdditiveAnimations.size();
	auto& animation = myAdditiveAnimations.emplace_back(anAnimation);
	animation->SetAdditiveAnimation(true);
	animation->SetTargetFrameDelta(myAnimationDelta);
	animation->Init(myBoneTransformCache, mySkeleton);
	return index;
}

void AnimationControllerComponent::RemoveAnimation(size_t anIndex)
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
		myAnimation->ResetTimer();
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
	if (myAnimation)
	{
		myAnimation->SetTargetFrameDelta(myAnimationDelta);
		for (auto& animation : myAdditiveAnimations)
		{
			animation->SetTargetFrameDelta(myAnimationDelta);
		}
	}
}

void AnimationControllerComponent::UpdateBoneCache()
{
	ResetBoneCache();
	if (myAnimation)
	{
		myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache, myAnimationTimer / myAnimationDelta);
		for (auto& animation : myAdditiveAnimations)
		{
			animation->UpdateBoneCache(mySkeleton, myBoneTransformCache, myAnimationTimer / myAnimationDelta);
		}
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
