#include "AssetManager.pch.h"
#include "AnimationLayer.h"
#include "Skeleton.h"

AnimationLayer::AnimationLayer() : Animation(AnimationType::AnimationLayer), myBoneIndex(0u)
{}

AnimationLayer::AnimationLayer(const Animation& anAnimation, unsigned aBoneIndex) : Animation(anAnimation), myBoneIndex(aBoneIndex)
{
	myType = AnimationType::AnimationLayer;
}

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
	const auto& bone = aSkeleton->GetBone(myBoneIndex);
	if (bone.parent >= 0)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], aSkeleton->GetBone(bone.parent).bindPoseInverse.GetInverse());
	}
	else
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], Crimson::Matrix4x4f::Identity);
	}	
}

void AnimationLayer::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	Crimson::Matrix4x4f parentMatrix;
	const auto& bone = aSkeleton->GetBone(myBoneIndex);
	if (bone.parent >= 0)
	{
		parentMatrix = aSkeleton->GetBone(bone.parent).bindPoseInverse.GetInverse();
	}

	if (myIsPlayingInReverse)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], GetPreviousFrame(), anInterpolationValue, parentMatrix);
	}
	else
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], GetNextFrame(), anInterpolationValue, parentMatrix);
	}
}

std::shared_ptr<AnimationBase> AnimationLayer::GetAsSharedPtr() const
{
	return std::make_shared<AnimationLayer>(*this);
}

void AnimationLayer::UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f aParentTransform) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	const auto& matrix = aFrame.localTransformMatrices.at(bone.name) * aParentTransform;
	outBones[anIndex] = bone.bindPoseInverse * matrix;
	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, childIndex, aFrame, matrix);
	}
}

void AnimationLayer::UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f aParentTransform) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	const auto& interpolatedTransform = AnimationTransform::Interpolate(aCurrentFrame.localTransforms.at(bone.namespaceName), anInterpolationFrame.localTransforms.at(bone.namespaceName), anInterpolationValue);
	const auto& matrix = interpolatedTransform.GetAsMatrix() * aParentTransform;
	outBones[anIndex] = bone.bindPoseInverse * matrix;
	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, childIndex, aCurrentFrame, anInterpolationFrame, anInterpolationValue, matrix);
	}
}
