#include "AssetManager.pch.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Time/Timer.h"
#include "AssetManager.h"

Animation::Animation() :AnimationBase(AnimationType::Animation), myData(nullptr), myCurrentFrame(1)
{}

Animation::Animation(const AnimationData* someData) : AnimationBase(AnimationType::Animation), myData(someData), myCurrentFrame(1)
{}

Animation::Animation(const Animation& anAnimation) : AnimationBase(anAnimation), myData(anAnimation.myData), myCurrentFrame(anAnimation.myCurrentFrame)
{}

Animation::Animation(Animation&& anAnimation) noexcept : AnimationBase(std::move(anAnimation)), myData(anAnimation.myData), myCurrentFrame(anAnimation.myCurrentFrame)
{}

Animation::Animation(AnimationType aType) : AnimationBase(aType), myData(nullptr), myCurrentFrame(1)
{}

bool Animation::operator==(const Animation& anAnimation) const
{
	return myData == anAnimation.myData;
}

bool Animation::Update()
{
	if (!myData)
	{
		return false;
	}

	myAnimationTimer += Crimson::Timer::GetDeltaTime();
	if (myAnimationTimer >= myData->frameDelta)
	{
		myInterpolationTimer = 0.f;
		do
		{
			myAnimationTimer -= myData->frameDelta;
			const bool hasMoreFrames = myFlags[eIsReversing] ? PreviousFrame() : NextFrame();
			if (!hasMoreFrames && !myFlags[eIsLooping])
			{
				myFlags[eIsPlaying] = false;
				myAnimationTimer = 0.f;
				break;
			}
		} while (myAnimationTimer >= myData->frameDelta);

		if (mySkeleton && myBoneCache)
		{
			UpdateBoneCache(mySkeleton, *myBoneCache);
		}
	}
	else if (myTargetFrameDelta > 0.f)
	{
		myInterpolationTimer += Crimson::Timer::GetDeltaTime();
		if (myInterpolationTimer >= myTargetFrameDelta)
		{
			myInterpolationTimer -= myTargetFrameDelta;

			if (mySkeleton && myBoneCache)
			{
				UpdateBoneCache(mySkeleton, *myBoneCache, myAnimationTimer / myData->frameDelta);
			}
		}
	}
	return myFlags[eIsPlaying];
}

void Animation::Init(const Json::Value& aJson)
{
	AnimationBase::Init(aJson);
	myData = AssetManager::GetAsset<AnimationData*>(aJson["Path"].asString());
}

const std::string& Animation::GetPath() const
{
	return myData->name;
}

unsigned Animation::GetStartBoneIndex() const
{
	return 0u;
}

float Animation::GetFPS() const
{
	return myData->framesPerSecond;
}

float Animation::GetFrameDelta() const
{
	return myData->frameDelta;
}

unsigned Animation::GetFrameCount() const
{
	return myData->length;
}

void Animation::SetToFirstFrame()
{
	ResetTimer();
	myCurrentFrame = 1;
}

void Animation::SetToLastFrame()
{
	ResetTimer();
	myCurrentFrame = myData->length - 1;
}

void Animation::SetFrameIndex(unsigned anIndex)
{
	ResetTimer();
	myCurrentFrame = Crimson::Min(anIndex, GetLastFrameIndex());
}

bool Animation::NextFrame()
{
	ResetTimer();
	if (++myCurrentFrame == myData->length)
	{
		SetToFirstFrame();
	}
	if (myCurrentFrame == myData->length - 1)
	{
		return false;
	}
	return true;
}

bool Animation::PreviousFrame()
{
	ResetTimer();
	if (--myCurrentFrame == 0)
	{
		SetToLastFrame();
	}
	if (myCurrentFrame == 1)
	{
		return false;
	}
	return true;
}

void Animation::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const
{
	UpdateBoneCacheInternal(aSkeleton, outBones, 0u, myData->frames[myCurrentFrame]);
}

void Animation::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	if (myFlags[eIsReversing])
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, 0u, myData->frames[myCurrentFrame], GetPreviousFrame(), anInterpolationValue);
	}
	else
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, 0u, myData->frames[myCurrentFrame], GetNextFrame(), anInterpolationValue);
	}
}

const AnimationFrame& Animation::GetFrame(unsigned int anIndex) const
{
	return myData->frames[anIndex];
}

const AnimationFrame& Animation::GetCurrentFrame() const
{
	return myData->frames[myCurrentFrame];
}

const AnimationFrame& Animation::GetNextFrame() const
{
	unsigned nextFrame = myCurrentFrame + 1;
	if (nextFrame == myData->length)
	{
		nextFrame = 1;
	}

	return myData->frames[nextFrame];
}

const AnimationFrame& Animation::GetPreviousFrame() const
{
	unsigned previousFrame = myCurrentFrame - 1;
	if (previousFrame == 0)
	{
		previousFrame = GetLastFrameIndex();
	}

	return myData->frames[previousFrame];
}

unsigned Animation::GetLastFrameIndex() const
{
	return myData->length - 1;
}

unsigned Animation::GetCurrentFrameIndex() const
{
	return myCurrentFrame;
}

bool Animation::IsEndOfLoop() const
{
	return myFlags[eIsReversing] ? myCurrentFrame == 1 : myCurrentFrame == GetLastFrameIndex();
}

bool Animation::IsValid() const
{
	return myData && AnimationBase::IsValid();
}

bool Animation::HasData() const
{
	return myData;
}

bool Animation::IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage) const
{
	if (aSkeleton == nullptr || !HasData())
	{
		if (outErrorMessage)
		{
			*outErrorMessage = "Missing Animation or Skeleton";
		}
		return false;
	}

	const auto& frame = GetFrame(0);
	if (aSkeleton->GetBoneCount() != frame.globalTransformMatrices.size())
	{
		if (outErrorMessage)
		{
			*outErrorMessage = "Different Bonecounts! \nBones in Animation: " + std::to_string(frame.globalTransformMatrices.size());
		}
		return false;
	}

	for (auto& bone : aSkeleton->GetBones())
	{
		if (frame.globalTransformMatrices.find(bone.name) == frame.globalTransformMatrices.end() && frame.globalTransformMatrices.find(bone.namespaceName) == frame.globalTransformMatrices.end())
		{
			if (outErrorMessage)
			{
				*outErrorMessage = "Bone not found in animation! \nBone: " + bone.namespaceName;
			}
			return false;
		}
	}

	return true;
}

bool Animation::IsUsingNamespace(const Skeleton* aSkeleton) const
{
	const auto& frame = myData->frames.back();
	if (frame.globalTransforms.find(aSkeleton->GetBone(0).namespaceName) != frame.globalTransforms.end())
	{
		return true;
	}
	return false;
}

const AnimationData& Animation::GetData() const
{
	return *myData;
}

std::unordered_map<std::string, AnimationTransform> Animation::GetFrameTransforms() const
{
	return myData->frames[myCurrentFrame].globalTransforms;
}

std::unordered_map<std::string, AnimationTransform> Animation::GetFrameTransforms(float anInterpolationValue) const
{
	std::unordered_map<std::string, AnimationTransform> result;
	const auto& current = myData->frames[myCurrentFrame];
	const auto& next = myFlags[eIsReversing] ? GetPreviousFrame() : GetNextFrame();
	result.reserve(current.globalTransforms.size());
	for (auto& [bone, transform] : current.globalTransforms)
	{
		result.emplace(bone, AnimationTransform::Interpolate(transform, next.globalTransforms.at(bone), anInterpolationValue));
	}
	return result;
}

std::shared_ptr<AnimationBase> Animation::GetAsSharedPtr() const
{
	return std::make_shared<Animation>(*this);
}

void Animation::UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	if (myFlags[eIsUsingNamespace])
	{
		outBones[anIndex] = bone.bindPoseInverse * myData->frames[myCurrentFrame].globalTransformMatrices.at(bone.namespaceName);
	}
	else
	{
		outBones[anIndex] = bone.bindPoseInverse * myData->frames[myCurrentFrame].globalTransformMatrices.at(bone.name);
	}

	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, childIndex, aFrame);
	}
}

void Animation::UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	if (myFlags[eIsUsingNamespace])
	{
		const auto& interpolatedTransform = AnimationTransform::Interpolate(aCurrentFrame.globalTransforms.at(bone.namespaceName), anInterpolationFrame.globalTransforms.at(bone.namespaceName), anInterpolationValue);
		outBones[anIndex] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();
	}
	else
	{
		const auto& interpolatedTransform = AnimationTransform::Interpolate(aCurrentFrame.globalTransforms.at(bone.name), anInterpolationFrame.globalTransforms.at(bone.name), anInterpolationValue);
		outBones[anIndex] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();
	}

	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, childIndex, aCurrentFrame, anInterpolationFrame, anInterpolationValue);
	}
}
