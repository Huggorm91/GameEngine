#include "AssetManager.pch.h"
#include "AnimationLayer.h"

AnimationLayer::AnimationLayer() : Animation(), myBoneIndex(0u)
{}

AnimationLayer::AnimationLayer(AnimationData& someData, unsigned aBoneIndex) : Animation(someData), myBoneIndex(aBoneIndex)
{}

AnimationLayer::AnimationLayer(const Animation& anAnimation, unsigned aBoneIndex) : Animation(anAnimation), myBoneIndex(aBoneIndex)
{}

void AnimationLayer::SetBoneIndex(unsigned anIndex)
{
	myBoneIndex = anIndex;
}

unsigned AnimationLayer::GetBoneIndex() const
{
	return myBoneIndex;
}

void AnimationLayer::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const
{
	UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame]);
}

void AnimationLayer::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	if (myIsPlayingInReverse)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], GetPreviousFrame(), anInterpolationValue);
	}
	else
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], GetNextFrame(), anInterpolationValue);
	}
}

std::shared_ptr<AnimationBase> AnimationLayer::GetAsSharedPtr() const
{
	return std::make_shared<AnimationLayer>(*this);
}
