#include "AssetManager.pch.h"
#include "AnimationLayer.h"

AnimationLayer::AnimationLayer() : myBoneIndex(0u)
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

void AnimationLayer::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue, bool anInterpolatePreviousFrame) const
{
	if (anInterpolatePreviousFrame)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], GetPreviousFrame(), anInterpolationValue);
	}
	else
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], GetNextFrame(), anInterpolationValue);
	}
}
