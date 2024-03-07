#include "AssetManager.pch.h"
#include "BlendSpace.h"
#include "Time/Timer.h"
#include "Math/Sort.hpp"

BlendSpace::BlendSpace() :
	AnimationBase(AnimationType::BlendSpace),
	myLongestAnimation(nullptr),
	myBlendValueGetter(nullptr),
	myBlendValue(0.f),
	myBlendDataCounter(0u),
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

void BlendSpace::AddAnimation(const Animation& anAnimation, float aBlendValue)
{
	myAnimations.emplace_back(BlendData(std::make_shared<Animation>(anAnimation), aBlendValue, ++myBlendDataCounter));
	AddInternal();
}

void BlendSpace::AddAnimation(const AnimationLayer& anAnimation, float aBlendValue)
{
	myAnimations.emplace_back(BlendData(std::make_shared<AnimationLayer>(anAnimation), aBlendValue, ++myBlendDataCounter));
	AddInternal();
}

void BlendSpace::AddAnimation(const Animation& anAnimation, unsigned aBoneIndex, float aBlendValue)
{
	myAnimations.emplace_back(BlendData(std::make_shared<AnimationLayer>(anAnimation, aBoneIndex), aBlendValue, ++myBlendDataCounter));
	AddInternal();
}

void BlendSpace::RemoveAnimation(const Animation& anAnimation, float aBlendValue)
{
	unsigned index = 0;
	for (auto iter = myAnimations.begin(); iter != myAnimations.end(); iter++)
	{
		if (*iter->animation == anAnimation && iter->blendValue == aBlendValue)
		{
			unsigned id = iter->id;
			if (myLongestAnimation == iter->animation.get())
			{
				myLongestAnimation = nullptr;
			}
			myAnimations.erase(iter);
			UpdateMatchingFPS();
			if (myHasMatchingFPS)
			{
				myTimers.clear();
				myLongestAnimation = nullptr;
			}
			else
			{
				myTimers.erase(id);
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
			return;
		}
		++index;
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
	for (auto& data : myAnimations)
	{
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
	assert(!"Not Implemented!");
	aSkeleton; outBones;
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
		myTimers.emplace(data.id, 0.f);
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
			if (myHasMatchingFPS)
			{
				CreateTimers();
			}
			myHasMatchingFPS = false;
			return;
		}
		previous = data.animation.get();
	}
	myHasMatchingFPS = true;
}

void BlendSpace::CreateTimers()
{
	myTimers.clear();
	myTimers.reserve(myAnimations.size());
	for (auto& data : myAnimations)
	{
		if (!myLongestAnimation || data.animation->GetData().length > myLongestAnimation->GetData().length)
		{
			myLongestAnimation = data.animation.get();
		}
		myTimers.emplace(data.id, myAnimationTimer);
	}
}

void BlendSpace::UpdateAnimations()
{
	const float deltaTime = Crimson::Timer::GetDeltaTime();
	for (auto& data : myAnimations)
	{
		auto& timer = myTimers.at(data.id);
		timer += deltaTime;

		while (timer >= data.animation->GetFrameDelta())
		{
			timer -= data.animation->GetFrameDelta();
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
