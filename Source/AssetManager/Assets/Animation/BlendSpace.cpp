#include "AssetManager.pch.h"
#include "BlendSpace.h"
#include "Time/Timer.h"
#include "Math/Sort.hpp"
#include "Skeleton.h"

BlendSpace::BlendSpace() :
	AnimationBase(AnimationType::BlendSpace),
	myLongestAnimation(nullptr),
	myBlendValueGetter(nullptr),
	myBlendValue(0.f),
	myBoneIndex(0u),
	myHasMatchingFPS(true)
{}

bool BlendSpace::Update()
{
	if (myAnimations.empty())
	{
		return false;
	}

	if (myBlendValueGetter)
	{
		myBlendValue = myBlendValueGetter();
	}

	myAnimationTimer += Crimson::Timer::GetDeltaTime();
	if (myHasMatchingFPS)
	{
		const float frameDelta = myAnimations.back().animation->GetFrameDelta();
		if (myAnimationTimer >= frameDelta)
		{
			myInterpolationTimer = 0.f;
			do
			{
				myAnimationTimer -= frameDelta;
				const bool hasMoreFrames = myIsPlayingInReverse ? PreviousFrame() : NextFrame();
				if (!hasMoreFrames && !myIsLooping)
				{
					myIsPlaying = false;
					myAnimationTimer = 0.f;
					break;
				}
			} while (myAnimationTimer >= frameDelta);

			UpdateBoneCache(mySkeleton, *myBoneCache);
		}
		else if (myTargetFrameDelta > 0.f)
		{
			myInterpolationTimer += Crimson::Timer::GetDeltaTime();
			if (myInterpolationTimer >= myTargetFrameDelta)
			{
				myInterpolationTimer -= myTargetFrameDelta;
				UpdateBoneCache(mySkeleton, *myBoneCache, myAnimationTimer / frameDelta);
			}
		}
	}
	else
	{
		myInterpolationTimer += Crimson::Timer::GetDeltaTime();
		if (myInterpolationTimer >= myTargetFrameDelta)
		{
			UpdateAnimations();

			myInterpolationTimer = 0.f;

			const unsigned current = myLongestAnimation->GetCurrentFrameIndex();
			bool isLastFrame = myIsPlayingInReverse ? current == 1 : current == myLongestAnimation->GetLastFrameIndex();

			if (isLastFrame && !myIsLooping)
			{
				myIsPlaying = false;
				myAnimationTimer = 0.f;
			}

			UpdateBoneCache(mySkeleton, *myBoneCache, myAnimationTimer / myTargetFrameDelta);
		}
	}
	return myIsPlaying;
}

const std::string& BlendSpace::GetName() const
{
	return myName;
}

unsigned BlendSpace::GetStartBoneIndex() const
{
	return myBoneIndex;
}

bool BlendSpace::AddAnimation(const Animation& anAnimation, float aBlendValue)
{
	if (myBoneIndex == 0u)
	{
		myAnimations.emplace_back(BlendData(std::make_shared<Animation>(anAnimation), aBlendValue));
		AddInternal();
		return true;
	}
	return false;
}

bool BlendSpace::AddAnimation(const AnimationLayer& anAnimation, float aBlendValue)
{
	if (myAnimations.empty())
	{
		myBoneIndex = anAnimation.GetStartBoneIndex();
		myAnimations.emplace_back(BlendData(std::make_shared<AnimationLayer>(anAnimation), aBlendValue));
		AddInternal();
		return true;
	}
	else
	{
		if (myBoneIndex == anAnimation.GetStartBoneIndex())
		{
			myAnimations.emplace_back(BlendData(std::make_shared<AnimationLayer>(anAnimation), aBlendValue));
			AddInternal();
			return true;
		}
	}
	return false;
}

bool BlendSpace::AddAnimation(const Animation& anAnimation, unsigned aBoneIndex, float aBlendValue)
{
	if (myAnimations.empty())
	{
		myBoneIndex = aBoneIndex;
		myAnimations.emplace_back(BlendData(std::make_shared<AnimationLayer>(anAnimation, aBoneIndex), aBlendValue));
		AddInternal();
		return true;
	}
	else
	{
		if (myBoneIndex == aBoneIndex)
		{
			myAnimations.emplace_back(BlendData(std::make_shared<AnimationLayer>(anAnimation, aBoneIndex), aBlendValue));
			AddInternal();
			return true;
		}
	}
	return false;
}

bool BlendSpace::RemoveAnimation(const Animation& anAnimation, float aBlendValue)
{
	unsigned index = 0;
	for (auto iter = myAnimations.begin(); iter != myAnimations.end(); iter++)
	{
		if (*iter->animation == anAnimation && iter->blendValue == aBlendValue)
		{
			if (myLongestAnimation == iter->animation.get())
			{
				myLongestAnimation = nullptr;
			}
			myAnimations.erase(iter);
			UpdateMatchingFPS();
			if (myHasMatchingFPS)
			{
				myLongestAnimation = nullptr;
			}
			else
			{
				if (myLongestAnimation == nullptr)
				{
					for (auto& data : myAnimations)
					{
						if (!myLongestAnimation || data.animation->GetData().length > myLongestAnimation->GetData().length)
						{
							myLongestAnimation = data.animation.get();
						}
					}
				}
			}
			return true;
		}
		++index;
	}
	return false;
}

void BlendSpace::SetToFirstFrame()
{
	ResetTimer();
	for (auto& data : myAnimations)
	{
		data.timer = 0.f;
		data.animation->SetToFirstFrame();
	}
}

void BlendSpace::SetToLastFrame()
{
	ResetTimer();
	for (auto& data : myAnimations)
	{
		data.timer = 0.f;
		data.animation->SetToLastFrame();
	}
}

bool BlendSpace::NextFrame()
{
	for (auto& data : myAnimations)
	{
		data.timer = 0.f;
		data.animation->NextFrame();
	}

	if (myHasMatchingFPS)
	{
		const auto& animation = myAnimations.back().animation;
		return animation->GetCurrentFrameIndex() == animation->GetLastFrameIndex();
	}
	else
	{
		return myLongestAnimation->GetCurrentFrameIndex() == myLongestAnimation->GetLastFrameIndex();
	}
}

bool BlendSpace::PreviousFrame()
{
	for (auto& data : myAnimations)
	{
		data.timer = 0.f;
		data.animation->PreviousFrame();
	}
	
	if (myHasMatchingFPS)
	{
		return myAnimations.back().animation->GetCurrentFrameIndex() == 1u;
	}
	else
	{
		return myLongestAnimation->GetCurrentFrameIndex() == 1u;
	}
}

void BlendSpace::SetBlendValueGetter(const std::function<float()>& aFunction)
{
	myBlendValueGetter = aFunction;
}

void BlendSpace::SetBlendValue(float aValue)
{
	myBlendValue = aValue;
}

void BlendSpace::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const
{
	const BlendData* previous = nullptr;
	for (auto& data : myAnimations)
	{
		if (myBlendValue <= data.blendValue)
		{
			// Check if lower than first value, or equal to higher value
			if (previous == nullptr || Crimson::GimbalCheck(data.blendValue, myBlendValue, Crimson::FloatTolerance))
			{
				data.animation->UpdateBoneCache(aSkeleton, outBones);
			}
			else
			{
				const auto& lowerTransforms = previous->animation->GetFrameTransforms();
				const auto& higherTransforms = data.animation->GetFrameTransforms();
				const float interpolationValue = (myBlendValue - previous->blendValue) / (data.blendValue - previous->blendValue);

				// Ensure it is always the bigger list that is iterated
				if (lowerTransforms.size() == higherTransforms.size())
				{
					for (auto& [boneName, transform] : lowerTransforms)
					{
						const unsigned index = aSkeleton->GetBoneIndex(boneName);
						const auto& bone = aSkeleton->GetBone(index);

						const auto& interpolatedTransform = AnimationTransform::Interpolate(transform, higherTransforms.at(boneName), interpolationValue);
						outBones[index] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();
					}
				}
				else if (lowerTransforms.size() < higherTransforms.size())
				{
					for (auto& [boneName, transform] : higherTransforms)
					{
						const unsigned index = aSkeleton->GetBoneIndex(boneName);
						const auto& bone = aSkeleton->GetBone(index);

						if (auto iter = lowerTransforms.find(boneName); iter != lowerTransforms.end())
						{
							const auto& interpolatedTransform = AnimationTransform::Interpolate(iter->second, transform, interpolationValue);
							outBones[index] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();
						}
						else
						{
							outBones[index] = bone.bindPoseInverse * transform.GetAsMatrix();
						}
					}
				}
				else
				{
					for (auto& [boneName, transform] : lowerTransforms)
					{
						const unsigned index = aSkeleton->GetBoneIndex(boneName);
						const auto& bone = aSkeleton->GetBone(index);

						if (auto iter = higherTransforms.find(boneName); iter != higherTransforms.end())
						{
							const auto& interpolatedTransform = AnimationTransform::Interpolate(transform, iter->second, interpolationValue);
							outBones[index] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();
						}
						else
						{
							outBones[index] = bone.bindPoseInverse * transform.GetAsMatrix();
						}
					}
				}
			}
		}
		previous = &data;
	}
}

void BlendSpace::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	assert(!"Not Implemented!");
	aSkeleton; outBones; anInterpolationValue;
}

bool BlendSpace::IsValid() const
{
	return HasData() && AnimationBase::IsValid();
}

bool BlendSpace::HasData() const
{
	return !myAnimations.empty();
}

void BlendSpace::SetIsPlayingInReverse(bool aShouldPlayBackwards)
{
	for (auto& data : myAnimations)
	{
		data.animation->SetIsPlayingInReverse(aShouldPlayBackwards);
	}
}

bool BlendSpace::IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage) const
{
	for (auto& data : myAnimations)
	{
		if (!data.animation->IsValidSkeleton(aSkeleton, outErrorMessage))
		{
			return false;
		}
	}
	return true;
}

std::shared_ptr<AnimationBase> BlendSpace::GetAsSharedPtr() const
{
	return std::make_shared<BlendSpace>(*this);
}

void BlendSpace::AddInternal()
{
	if (myHasMatchingFPS)
	{
		UpdateMatchingFPS();
	}
	else
	{
		const auto& data = myAnimations.back();
		if (data.animation->GetData().length > myLongestAnimation->GetData().length)
		{
			myLongestAnimation = data.animation.get();
		}
	}
	Crimson::QuickSort(myAnimations);
}

void BlendSpace::UpdateMatchingFPS()
{
	Animation* previous = nullptr;
	for (auto& data : myAnimations)
	{
		if (previous && data.animation->GetFPS() != previous->GetFPS())
		{
			myHasMatchingFPS = false;
			return;
		}
		previous = data.animation.get();
	}
	myHasMatchingFPS = true;
}

void BlendSpace::UpdateAnimations()
{
	const float deltaTime = Crimson::Timer::GetDeltaTime();
	for (auto& data : myAnimations)
	{
		data.timer += deltaTime;

		while (data.timer >= data.animation->GetFrameDelta())
		{
			data.timer -= data.animation->GetFrameDelta();
			if (myIsPlayingInReverse)
			{
				data.animation->PreviousFrame();
			}
			else
			{
				data.animation->NextFrame();
			}
		}
	}
}