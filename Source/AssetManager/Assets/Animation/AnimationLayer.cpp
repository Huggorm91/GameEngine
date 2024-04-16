#include "AssetManager.pch.h"
#include "AnimationLayer.h"
#include "Skeleton.h"

AnimationLayer::AnimationLayer() : Animation(AnimationType::AnimationLayer), myBoneIndex(0u)
{}

AnimationLayer::AnimationLayer(const Animation& anAnimation, unsigned aBoneIndex) : Animation(anAnimation), myBoneIndex(aBoneIndex)
{
	myType = AnimationType::AnimationLayer;
}

bool AnimationLayer::UpdateRootMotion(float)
{
	return false;
}

AnimationTransform AnimationLayer::GetRootMotion(float)
{
	return AnimationTransform();
}

void AnimationLayer::Init(const Json::Value& aJson)
{
	Animation::Init(aJson);
	myBoneIndex = aJson["BoneIndex"].asUInt();
}

unsigned AnimationLayer::GetStartBoneIndex() const
{
	return myBoneIndex;
}

void AnimationLayer::SetBoneIndex(unsigned anIndex)
{
	myBoneIndex = anIndex;
}

void AnimationLayer::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const
{
	const auto& bone = aSkeleton->GetBone(myBoneIndex);
	if (bone.parent >= 0)
	{
		UpdateBoneCacheLocal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], aSkeleton->GetBone(bone.parent).bindPoseInverse.GetInverse());
	}
	else
	{
		UpdateBoneCacheLocal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], Crimson::Matrix4x4f::Identity);
	}
}

void AnimationLayer::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	const auto& bone = aSkeleton->GetBone(myBoneIndex);
	const auto& next = myFlags[eIsReversing] ? GetPreviousFrame() : GetNextFrame();
	if (bone.parent >= 0)
	{
		UpdateBoneCacheLocal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], next, anInterpolationValue, aSkeleton->GetBone(bone.parent).bindPoseInverse.GetInverse());
	}
	else
	{
		UpdateBoneCacheLocal(aSkeleton, outBones, myBoneIndex, myData->frames[myCurrentFrame], next, anInterpolationValue, Crimson::Matrix4x4f::Identity);
	}
}

std::unordered_map<std::string, AnimationTransform> AnimationLayer::GetFrameTransforms() const
{
	std::unordered_map<std::string, AnimationTransform> result;
	const auto& bone = mySkeleton->GetBone(myBoneIndex);
	if (bone.parent >= 0)
	{
		GetFrameTransformsInternal(result, myBoneIndex, myData->frames[myCurrentFrame], mySkeleton->GetBone(bone.parent).bindPoseInverse.GetInverse());
	}
	else
	{
		GetFrameTransformsInternal(result, myBoneIndex, myData->frames[myCurrentFrame], Crimson::Matrix4x4f::Identity);
	}
	return result;
}

std::unordered_map<std::string, AnimationTransform> AnimationLayer::GetFrameTransforms(float anInterpolationValue) const
{
	std::unordered_map<std::string, AnimationTransform> result;
	const auto& next = myFlags[eIsReversing] ? GetPreviousFrame() : GetNextFrame();
	const auto& bone = mySkeleton->GetBone(myBoneIndex);
	if (bone.parent >= 0)
	{
		GetFrameTransformsInternal(result, myBoneIndex, myData->frames[myCurrentFrame], next, anInterpolationValue, mySkeleton->GetBone(bone.parent).bindPoseInverse.GetInverse());
	}
	else
	{
		GetFrameTransformsInternal(result, myBoneIndex, myData->frames[myCurrentFrame], next, anInterpolationValue, Crimson::Matrix4x4f::Identity);
	}
	return result;
}

std::shared_ptr<AnimationBase> AnimationLayer::GetAsSharedPtr() const
{
	return std::make_shared<AnimationLayer>(*this);
}

Json::Value AnimationLayer::ToJson() const
{
	Json::Value result = Animation::ToJson();
	result["BoneIndex"] = myBoneIndex;
	return result;
}

void AnimationLayer::GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const
{
	const auto& bone = mySkeleton->GetBone(anIndex);
	const std::string* name = &bone.namespaceName;
	if (!myFlags[eIsUsingNamespace])
	{
		name = &bone.name;
	}

	const auto& matrix = aFrame.localTransformMatrices.at(*name) * aParentTransform;
	outTransforms.emplace(*name, matrix);
	for (auto& childIndex : bone.children)
	{
		GetFrameTransformsInternal(outTransforms, childIndex, aFrame, matrix);
	}
}

void AnimationLayer::GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const
{
	const auto& bone = mySkeleton->GetBone(anIndex);
	const std::string* name = &bone.namespaceName;
	if (!myFlags[eIsUsingNamespace])
	{
		name = &bone.name;
	}

	const auto& interpolatedTransform = AnimationTransform::Interpolate(aCurrentFrame.localTransforms.at(*name), anInterpolationFrame.localTransforms.at(*name), anInterpolationValue);
	const auto& matrix = interpolatedTransform.GetAsMatrix() * aParentTransform;
	outTransforms.emplace(*name, matrix);
	
	for (auto& childIndex : bone.children)
	{
		GetFrameTransformsInternal(outTransforms, childIndex, aCurrentFrame, anInterpolationFrame, anInterpolationValue, matrix);
	}
}
