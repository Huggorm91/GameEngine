#include "AssetManager.pch.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Time/Timer.h"
#include "AssetManager.h"
#include "AnimationLayer.h"

Animation::Animation() :AnimationBase(AnimationType::Animation), myData(nullptr), myCurrentFrame(1)
{}

Animation::Animation(const AnimationData* someData) : AnimationBase(AnimationType::Animation), myData(someData), myCurrentFrame(1)
{}

Animation::Animation(const Animation& anAnimation) : AnimationBase(anAnimation), myData(anAnimation.myData), myCurrentFrame(anAnimation.myCurrentFrame)
{}

Animation::Animation(const AnimationLayer& anAnimation) : AnimationBase(anAnimation), myData(anAnimation.myData), myCurrentFrame(anAnimation.myCurrentFrame)
{
	myType = AnimationType::Animation;
}

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
	if (!myData || !myFlags[eIsPlaying])
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

		if (AnimationBase::IsValid() && !myFlags[eIsAdditive])
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

			if (AnimationBase::IsValid() && !myFlags[eIsAdditive])
			{
				UpdateBoneCache(mySkeleton, *myBoneCache, myAnimationTimer / myData->frameDelta);
			}
		}
	}
	return myFlags[eIsPlaying];
}

bool Animation::UpdateRootMotion(float aTimeSinceLastUpdate)
{
	if (!myRootMotionTransform)
	{
		return false;
	}

	const QuaternionTransform& transform = GetRootMotion(aTimeSinceLastUpdate / myData->frameDelta);
	if (transform != QuaternionTransform())
	{
		myRootMotionTransform->AddToPosition(transform.position);
		myRootMotionTransform->AddToRotationRadian(transform.rotation.GetAsEuler());
		return true;
	}
	return false;
}

QuaternionTransform Animation::GetRootMotion(float aPercentage)
{
	assert(mySkeleton && myRootMotionTransform && "Animation not configured correctly to use root motion!");
	const std::string* boneName = &mySkeleton->GetBone(0).namespaceName;
	if (!myFlags[eIsUsingNamespace])
	{
		boneName = &mySkeleton->GetBone(0).name;
	}

	const AnimationFrame* previousFrame = nullptr;

	if (myFlags[eIsReversing])
	{
		if (myCurrentFrame != GetLastFrameIndex())
		{
			previousFrame = &GetNextFrame();
		}
		else
		{
			previousFrame = &myData->frames[myCurrentFrame]; // This does not work correctly but it looks good enough on my current test animation.
		}
	}
	else
	{
		if (myCurrentFrame != 1u)
		{
			previousFrame = &GetPreviousFrame();
		}
	}

	const auto& previousTransform = previousFrame ? previousFrame->localTransforms.at(*boneName) : QuaternionTransform();
	const auto& rootTransform = myData->frames[myCurrentFrame].localTransforms.at(*boneName);

	auto transform = QuaternionTransform::Interpolate(previousTransform, rootTransform, aPercentage);
	transform.Subtract(previousTransform);
	return transform;
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
	return myTargetFrameDelta > 0.f ? myTargetFrameDelta : myData->frameDelta;
}

unsigned Animation::GetFrameCount() const
{
	return myData->length;
}

void Animation::SetToFirstFrame()
{
	myCurrentFrame = 1;
}

void Animation::SetToLastFrame()
{
	myCurrentFrame = myData->length - 1;
}

void Animation::SetFrameIndex(unsigned anIndex)
{
	myCurrentFrame = Crimson::Min(anIndex, GetLastFrameIndex());
}

bool Animation::NextFrame()
{
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
	if (myRootMotionTransform)
	{
		outBones[0] = Crimson::Matrix4x4f::Identity;
		for (auto& childIndex : aSkeleton->GetBone(0u).children)
		{
			UpdateBoneCacheLocal(aSkeleton, outBones, childIndex, myData->frames[myCurrentFrame], Crimson::Matrix4x4f::Identity);
		}
	}
	else
	{
		UpdateBoneCacheGlobal(aSkeleton, outBones, 0u, myData->frames[myCurrentFrame]);
	}
}

void Animation::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	const AnimationFrame* nextFrame = nullptr;

	if (myFlags[eIsReversing])
	{
		nextFrame = &GetPreviousFrame();
	}
	else
	{
		nextFrame = &GetNextFrame();
	}

	if (myRootMotionTransform)
	{
		outBones[0] = Crimson::Matrix4x4f::Identity;
		for (auto& childIndex : aSkeleton->GetBone(0u).children)
		{
			UpdateBoneCacheLocal(aSkeleton, outBones, childIndex, myData->frames[myCurrentFrame], *nextFrame, anInterpolationValue, Crimson::Matrix4x4f::Identity);
		}
	}
	else
	{
		UpdateBoneCacheGlobal(aSkeleton, outBones, 0u, myData->frames[myCurrentFrame], *nextFrame, anInterpolationValue);
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
	const bool usingNameSpace = frame.globalTransforms.find(aSkeleton->GetBone(0).namespaceName) != frame.globalTransforms.end();
	if (aSkeleton->GetBoneCount() != frame.globalTransformMatrices.size())
	{
		if (outErrorMessage)
		{
			*outErrorMessage = "Different Bonecounts! \nBones in Animation: " + std::to_string(frame.globalTransformMatrices.size());

			if (aSkeleton->GetBoneCount() < frame.globalTransformMatrices.size())
			{
				*outErrorMessage += "\n\nBones missing in Skeleton:";
				auto remainingBones = frame.globalTransformMatrices;

				if (usingNameSpace)
				{
					for (auto& bone : aSkeleton->GetBones())
					{
						remainingBones.erase(bone.namespaceName);
					}
				}
				else
				{
					for (auto& bone : aSkeleton->GetBones())
					{
						remainingBones.erase(bone.name);
					}
				}

				for (auto& [boneName, matrix] : remainingBones)
				{
					*outErrorMessage += "\n" + boneName;
				}
			}
			else
			{
				*outErrorMessage += "\n\nBones missing in Animation:\n";
				if (usingNameSpace)
				{
					for (auto& bone : aSkeleton->GetBones())
					{
						if (frame.globalTransformMatrices.find(bone.namespaceName) == frame.globalTransformMatrices.end())
						{
							*outErrorMessage += "\n" + bone.namespaceName;
						}
					}
				}
				else
				{
					for (auto& bone : aSkeleton->GetBones())
					{
						if (frame.globalTransformMatrices.find(bone.name) == frame.globalTransformMatrices.end())
						{
							*outErrorMessage += "\n" + bone.name;
						}
					}
				}
			}
		}
		return false;
	}

	if (usingNameSpace)
	{
		for (auto& bone : aSkeleton->GetBones())
		{
			if (frame.globalTransformMatrices.find(bone.namespaceName) == frame.globalTransformMatrices.end())
			{
				if (outErrorMessage)
				{
					*outErrorMessage = "Bone not found in animation! \nBone: " + bone.namespaceName;
				}
				return false;
			}
		}
	}
	else
	{
		for (auto& bone : aSkeleton->GetBones())
		{
			if (frame.globalTransformMatrices.find(bone.name) == frame.globalTransformMatrices.end())
			{
				if (outErrorMessage)
				{
					*outErrorMessage = "Bone not found in animation! \nBone: " + bone.name;
				}
				return false;
			}
		}
	}

	return true;
}

const AnimationData& Animation::GetData() const
{
	return *myData;
}

std::unordered_map<std::string, QuaternionTransform> Animation::GetAdditiveTransforms() const
{
	std::unordered_map<std::string, QuaternionTransform> result;
	auto transforms = myData->frames[myCurrentFrame].localTransformMatrices;
	const std::string* boneName = nullptr;
	for (auto& bone : mySkeleton->GetBones())
	{
		if (myFlags[eIsUsingNamespace])
		{
			boneName = &bone.namespaceName;
		}
		else
		{
			boneName = &bone.name;
		}

		if (auto iter = transforms.find(*boneName); iter != transforms.end())
		{
			result.emplace(iter->first, bone.localBindPoseInverse * iter->second);
		}
	}
	return result;
	/*if (myInterpolationTimer <= Crimson::FloatTolerance)
	{
		auto transforms = myData->frames[myCurrentFrame].localTransformMatrices;
		const std::string* boneName = nullptr;
		for (auto& bone : mySkeleton->GetBones())
		{
			if (myFlags[eIsUsingNamespace])
			{
				boneName = &bone.namespaceName;
			}
			else
			{
				boneName = &bone.name;
			}

			if (auto iter = transforms.find(*boneName); iter != transforms.end())
			{
				result.emplace(iter->first, iter->second * bone.localBindPoseInverse);
			}
		}
		return result;
	}
	else
	{

		const auto& current = myData->frames[myCurrentFrame];
		const auto& next = myFlags[eIsReversing] ? GetPreviousFrame() : GetNextFrame();
		result.reserve(current.globalTransforms.size());

		const float interpolationValue = myAnimationTimer / myData->frameDelta;
		const std::string* boneName = nullptr;
		for (auto& bone : mySkeleton->GetBones())
		{
			if (myFlags[eIsUsingNamespace])
			{
				boneName = &bone.namespaceName;
			}
			else
			{
				boneName = &bone.name;
			}

			if (auto iter = current.globalTransforms.find(*boneName); iter != current.globalTransforms.end())
			{
				QuaternionTransform transform = QuaternionTransform::Interpolate(iter->second, next.globalTransforms.at(iter->first), interpolationValue);
				transform.Subtract(bone.bindPose);
				result.emplace(iter->first, transform);
			}
		}
		return result;
	}*/
}

std::unordered_map<std::string, QuaternionTransform> Animation::GetFrameTransforms() const
{
	return myData->frames[myCurrentFrame].globalTransforms;
}

std::unordered_map<std::string, QuaternionTransform> Animation::GetFrameTransforms(float anInterpolationValue) const
{
	std::unordered_map<std::string, QuaternionTransform> result;
	const auto& current = myData->frames[myCurrentFrame];
	const auto& next = myFlags[eIsReversing] ? GetPreviousFrame() : GetNextFrame();
	result.reserve(current.globalTransforms.size());
	for (auto& [bone, transform] : current.globalTransforms)
	{
		result.emplace(bone, QuaternionTransform::Interpolate(transform, next.globalTransforms.at(bone), anInterpolationValue));
	}
	return result;
}

std::shared_ptr<AnimationBase> Animation::GetAsSharedPtr() const
{
	return std::make_shared<Animation>(*this);
}

void Animation::ValidateUsingNamespace(const Skeleton* aSkeleton)
{
	const auto& transforms = myData->frames.back().globalTransforms;
	myFlags[eIsUsingNamespace] = transforms.find(aSkeleton->GetBone(0).namespaceName) != transforms.end();
}

void Animation::UpdateBoneCacheGlobal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	const std::string* boneName = &bone.namespaceName;
	if (!myFlags[eIsUsingNamespace])
	{
		boneName = &bone.name;
	}

	outBones[anIndex] = bone.bindPoseInverse * myData->frames[myCurrentFrame].globalTransformMatrices.at(*boneName);

	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheGlobal(aSkeleton, outBones, childIndex, aFrame);
	}
}

void Animation::UpdateBoneCacheGlobal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	const std::string* boneName = &bone.namespaceName;
	if (!myFlags[eIsUsingNamespace])
	{
		boneName = &bone.name;
	}

	const auto& interpolatedTransform = QuaternionTransform::Interpolate(aCurrentFrame.globalTransforms.at(*boneName), anInterpolationFrame.globalTransforms.at(*boneName), anInterpolationValue);
	outBones[anIndex] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();

	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheGlobal(aSkeleton, outBones, childIndex, aCurrentFrame, anInterpolationFrame, anInterpolationValue);
	}
}

void Animation::UpdateBoneCacheLocal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	const std::string* name = &bone.namespaceName;
	if (!myFlags[eIsUsingNamespace])
	{
		name = &bone.name;
	}

	const auto& matrix = aFrame.localTransformMatrices.at(*name) * aParentTransform;
	outBones[anIndex] = bone.bindPoseInverse * matrix;

	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheLocal(aSkeleton, outBones, childIndex, aFrame, matrix);
	}
}

void Animation::UpdateBoneCacheLocal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	const std::string* name = &bone.namespaceName;
	if (!myFlags[eIsUsingNamespace])
	{
		name = &bone.name;
	}

	const auto& interpolatedTransform = QuaternionTransform::Interpolate(aCurrentFrame.localTransforms.at(*name), anInterpolationFrame.localTransforms.at(*name), anInterpolationValue);
	const auto& matrix = interpolatedTransform.GetAsMatrix() * aParentTransform;
	outBones[anIndex] = bone.bindPoseInverse * matrix;

	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheLocal(aSkeleton, outBones, childIndex, aCurrentFrame, anInterpolationFrame, anInterpolationValue, matrix);
	}
}
