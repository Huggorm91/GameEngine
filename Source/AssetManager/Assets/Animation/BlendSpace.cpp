#include "AssetManager.pch.h"
#include "BlendSpace.h"
#include "Time/Timer.h"
#include "Math/Sort.hpp"

BlendSpace::BlendSpace() : AnimationBase(AnimationType::BlendSpace), myBlendValueGetter(nullptr), myBlendValue(0.f)
{}

bool BlendSpace::Update()
{
	if (myBlendValueGetter)
	{
		myBlendValue = myBlendValueGetter();
	}

	myAnimationTimer += Crimson::Timer::GetDeltaTime();
	bool isLastFrame = false;
	/*if (myAnimationTimer >= myData->frameDelta)
	{
		myInterpolationTimer = 0.f;
		while (myAnimationTimer >= myData->frameDelta)
		{
			myAnimationTimer -= myData->frameDelta;
			const bool hasMoreFrames = myIsPlayingInReverse ? PreviousFrame() : NextFrame();
			if (!hasMoreFrames)
			{
				isLastFrame = true;
				if (!myIsLooping)
				{
					myIsPlaying = false;
					myAnimationTimer = 0.f;
					break;
				}
			}
		}
		UpdateBoneCache(mySkeleton, *myBoneCache);
	}
	else if (myTargetFrameDelta > 0.f)
	{
		myInterpolationTimer += Crimson::Timer::GetDeltaTime();
		if (myInterpolationTimer >= myTargetFrameDelta)
		{
			myInterpolationTimer -= myTargetFrameDelta;
			UpdateBoneCache(mySkeleton, *myBoneCache, myAnimationTimer / myData->frameDelta);
		}
	}*/
	return isLastFrame;
}

const std::string& BlendSpace::GetName() const
{
	return myName;
}

void BlendSpace::AddAnimation(const Animation& anAnimation, float aBlendValue)
{
	myAnimations.emplace_back(BlendData{ new Animation(anAnimation), aBlendValue });
	Crimson::QuickSort(myAnimations);
}

void BlendSpace::AddAnimation(const AnimationLayer& anAnimation, float aBlendValue)
{
	myAnimations.emplace_back(BlendData{ new AnimationLayer(anAnimation), aBlendValue });
	Crimson::QuickSort(myAnimations);
}

void BlendSpace::AddAnimation(const Animation& anAnimation, unsigned aBoneIndex, float aBlendValue)
{
	myAnimations.emplace_back(BlendData{ new AnimationLayer(anAnimation, aBoneIndex), aBlendValue });
	Crimson::QuickSort(myAnimations);
}

void BlendSpace::RemoveAnimation(const Animation& anAnimation, float aBlendValue)
{
	for (auto iter = myAnimations.begin(); iter != myAnimations.end(); iter++)
	{
		if (*iter->animation == anAnimation && iter->blendValue == aBlendValue)
		{
			myAnimations.erase(iter);
			return;
		}
	}
}

void BlendSpace::SetToFirstFrame()
{
	ResetTimer();
	for (auto& data : myAnimations)
	{
		data.animation->SetToFirstFrame();
	}
}

void BlendSpace::SetToLastFrame()
{
	ResetTimer();
	for (auto& data : myAnimations)
	{
		data.animation->SetToLastFrame();
	}
}

bool BlendSpace::NextFrame()
{
	ResetTimer();
	for (auto& data : myAnimations)
	{
		data.animation->NextFrame();
	}
	return false;
}

bool BlendSpace::PreviousFrame()
{
	ResetTimer();
	for (auto& data : myAnimations)
	{
		data.animation->PreviousFrame();
	}
	return false;
}

void BlendSpace::SetBlendValue(float aValue)
{
	myBlendValue = aValue;
}

void BlendSpace::SetBlendValueGetter(const std::function<float()>& aFunction)
{
	myBlendValueGetter = aFunction;
}

void BlendSpace::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const
{
	aSkeleton; outBones;
}

void BlendSpace::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	aSkeleton; outBones; anInterpolationValue;
	;
}

bool BlendSpace::IsValid() const
{
	return HasData() && AnimationBase::IsValid();
}

bool BlendSpace::HasData() const
{
	return !myAnimations.empty();
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
