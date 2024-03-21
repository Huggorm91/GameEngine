#include "AssetManager.pch.h"
#include "BlendSpace.h"
#include "Time/Timer.h"
#include "Math/Sort.hpp"
#include "Skeleton.h"
#include "AssetManager.h"

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
	if (myAnimations.empty() || !myFlags[eIsPlaying])
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
				const bool hasMoreFrames = myFlags[eIsReversing] ? PreviousFrame() : NextFrame();
				if (!hasMoreFrames && !myFlags[eIsLooping])
				{
					myFlags[eIsPlaying] = false;
					myAnimationTimer = 0.f;
					break;
				}
			} while (myAnimationTimer >= frameDelta);

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
					UpdateBoneCache(mySkeleton, *myBoneCache, myAnimationTimer / frameDelta);
				}
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
			bool isLastFrame = myFlags[eIsReversing] ? current == 1 : current == myLongestAnimation->GetLastFrameIndex();

			if (isLastFrame && !myFlags[eIsLooping])
			{
				myFlags[eIsPlaying] = false;
				myAnimationTimer = 0.f;
			}

			if (mySkeleton && myBoneCache)
			{
				UpdateBoneCache(mySkeleton, *myBoneCache, myAnimationTimer / myTargetFrameDelta);
			}
		}
	}
	return myFlags[eIsPlaying];
}

void BlendSpace::Init(BoneCache& aBoneCache, const Skeleton* aSkeleton)
{
	AnimationBase::Init(aBoneCache, aSkeleton);
	for (auto& data : myAnimations)
	{
		data.animation->Init(aBoneCache, aSkeleton);
	}
}

void BlendSpace::Init(const Json::Value& aJson)
{
	AnimationBase::Init(aJson);
	myPath = aJson["Path"].asString();
	myName = aJson["Name"].asString();
	myBlendValue = aJson["BlendValue"].asFloat();
	myBoneIndex = aJson["BoneIndex"].asUInt();

	if (myBoneIndex == 0u)
	{
		for (auto& data : aJson["Animations"])
		{
			AddAnimation(AssetManager::GetAsset<Animation>(data["Path"].asString()), data["Value"].asFloat());
		}
	}
	else
	{
		for (auto& data : aJson["Animations"])
		{
			AddAnimation(AssetManager::GetAsset<Animation>(data["Path"].asString()), myBoneIndex, data["Value"].asFloat());
		}
	}
}

const std::string& BlendSpace::GetName() const
{
	return myName;
}

const std::string& BlendSpace::GetPath() const
{
	return myPath;
}

unsigned BlendSpace::GetStartBoneIndex() const
{
	return myBoneIndex;
}

void BlendSpace::SetBoneIndex(unsigned anIndex)
{
	myBoneIndex = anIndex;

	auto tempAnimations = myAnimations;
	myAnimations.clear();
	if (anIndex == 0)
	{
		for (auto& data : tempAnimations)
		{
			data.animation = std::make_shared<Animation>(&data.animation->GetData());
			myAnimations.emplace_back(BlendData(data));
		}
	}
	else
	{
		for (auto& data : tempAnimations)
		{
			data.animation = std::make_shared<AnimationLayer>(*data.animation, myBoneIndex);
			myAnimations.emplace_back(BlendData(data));
		}
	}
}

bool BlendSpace::AddAnimation(const Animation& anAnimation, float aBlendValue, std::string* outErrorMessage)
{
	if (myBoneIndex == 0u)
	{
		myAnimations.emplace_back(BlendData(std::make_shared<Animation>(anAnimation), aBlendValue));
		AddInternal();
		return true;
	}
	else if (outErrorMessage)
	{
		*outErrorMessage = "Incorrect bone index!";
	}
	return false;
}

bool BlendSpace::AddAnimation(const AnimationLayer& anAnimation, float aBlendValue, std::string* outErrorMessage)
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
		else if (outErrorMessage)
		{
			*outErrorMessage = "Incorrect bone index!";
		}
	}
	return false;
}

bool BlendSpace::AddAnimation(const Animation& anAnimation, unsigned aBoneIndex, float aBlendValue, std::string* outErrorMessage)
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
		else if (outErrorMessage)
		{
			*outErrorMessage = "Incorrect bone index!";
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
	if (myAnimations.empty())
	{
		return;
	}

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

				for (auto& [boneName, transform] : lowerTransforms)
				{
					const unsigned index = aSkeleton->GetBoneIndex(boneName);
					const auto& bone = aSkeleton->GetBone(index);

					const auto& interpolatedTransform = AnimationTransform::Interpolate(transform, higherTransforms.at(boneName), interpolationValue);
					outBones[index] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();
				}
			}
			return;
		}
		previous = &data;
	}

	// Current blendvalue is higher than last animations blendvalue
	assert(previous && "Previous was nullptr");
	previous->animation->UpdateBoneCache(aSkeleton, outBones);
}

void BlendSpace::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const
{
	if (myAnimations.empty())
	{
		return;
	}

	const BlendData* previous = nullptr;
	for (auto& data : myAnimations)
	{
		if (myBlendValue <= data.blendValue)
		{
			// Check if lower than first value, or equal to higher value
			if (previous == nullptr || Crimson::GimbalCheck(data.blendValue, myBlendValue, Crimson::FloatTolerance))
			{
				data.animation->UpdateBoneCache(aSkeleton, outBones, anInterpolationValue);
			}
			else
			{
				const auto& lowerTransforms = previous->animation->GetFrameTransforms(anInterpolationValue);
				const auto& higherTransforms = data.animation->GetFrameTransforms(anInterpolationValue);
				const float interpolationValue = (myBlendValue - previous->blendValue) / (data.blendValue - previous->blendValue);

				for (auto& [boneName, transform] : lowerTransforms)
				{
					const unsigned index = aSkeleton->GetBoneIndex(boneName);
					const auto& bone = aSkeleton->GetBone(index);

					const auto& interpolatedTransform = AnimationTransform::Interpolate(transform, higherTransforms.at(boneName), interpolationValue);
					outBones[index] = bone.bindPoseInverse * interpolatedTransform.GetAsMatrix();
				}
			}
			return;
		}
		previous = &data;
	}

	// Current blendvalue is higher than last animations blendvalue
	assert(previous && "Previous was nullptr");
	previous->animation->UpdateBoneCache(aSkeleton, outBones, anInterpolationValue);
}

std::unordered_map<std::string, AnimationTransform> BlendSpace::GetFrameTransforms() const
{
	if (myAnimations.empty())
	{
		return std::unordered_map<std::string, AnimationTransform>();
	}

	const BlendData* previous = nullptr;
	for (auto& data : myAnimations)
	{
		if (myBlendValue <= data.blendValue)
		{
			// Check if lower than first value, or equal to higher value
			if (previous == nullptr || Crimson::GimbalCheck(data.blendValue, myBlendValue, Crimson::FloatTolerance))
			{
				return data.animation->GetFrameTransforms();
			}
			else
			{
				const auto& lowerTransforms = previous->animation->GetFrameTransforms();
				const auto& higherTransforms = data.animation->GetFrameTransforms();
				const float interpolationValue = (myBlendValue - previous->blendValue) / (data.blendValue - previous->blendValue);

				std::unordered_map<std::string, AnimationTransform> result;
				result.reserve(higherTransforms.size());

				for (auto& [boneName, transform] : lowerTransforms)
				{
					result.emplace(boneName, AnimationTransform::Interpolate(transform, higherTransforms.at(boneName), interpolationValue));
				}
				return result;
			}			
		}
		previous = &data;
	}

	// Current blendvalue is higher than last animations blendvalue
	assert(previous && "Previous was nullptr");
	return previous->animation->GetFrameTransforms();
}

std::unordered_map<std::string, AnimationTransform> BlendSpace::GetFrameTransforms(float anInterpolationValue) const
{
	if (myAnimations.empty())
	{
		return std::unordered_map<std::string, AnimationTransform>();
	}

	const BlendData* previous = nullptr;
	for (auto& data : myAnimations)
	{
		if (myBlendValue <= data.blendValue)
		{
			// Check if lower than first value, or equal to higher value
			if (previous == nullptr || Crimson::GimbalCheck(data.blendValue, myBlendValue, Crimson::FloatTolerance))
			{
				return data.animation->GetFrameTransforms(anInterpolationValue);
			}
			else
			{
				const auto& lowerTransforms = previous->animation->GetFrameTransforms(anInterpolationValue);
				const auto& higherTransforms = data.animation->GetFrameTransforms(anInterpolationValue);
				const float interpolationValue = (myBlendValue - previous->blendValue) / (data.blendValue - previous->blendValue);

				std::unordered_map<std::string, AnimationTransform> result;
				result.reserve(higherTransforms.size());

				for (auto& [boneName, transform] : lowerTransforms)
				{
					result.emplace(boneName, AnimationTransform::Interpolate(transform, higherTransforms.at(boneName), interpolationValue));
				}
				return result;
			}
		}
		previous = &data;
	}

	// Current blendvalue is higher than last animations blendvalue
	assert(previous && "Previous was nullptr");
	return previous->animation->GetFrameTransforms(anInterpolationValue);
}

bool BlendSpace::IsEndOfLoop() const
{
	return myLongestAnimation->IsEndOfLoop();
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
	if (myAnimations.empty())
	{
		return false;
	}

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

Json::Value BlendSpace::ToJson() const
{
	Json::Value result = AnimationBase::ToJson();
	result["Name"] = myName;
	result["BoneIndex"] = myBoneIndex;
	result["BlendValue"] = myBlendValue;
	result["Animations"] = Json::arrayValue;
	unsigned index = 0;
	for (auto& data : myAnimations)
	{
		auto& json = result["Animations"][index];
		json["Path"] = data.animation->GetPath();
		json["Value"] = data.blendValue;
		++index;
	}
	return result;
}

void BlendSpace::LoadFromJson(const Json::Value& aJson, const std::string& aPath)
{
	myPath = aPath;
	myName = aJson["Name"].asString();
	myBoneIndex = aJson["BoneIndex"].asUInt();

	if (myBoneIndex == 0u)
	{
		for (auto& data : aJson["Animations"])
		{
			AddAnimation(AssetManager::GetAsset<Animation>(data["Path"].asString()), data["Value"].asFloat());
		}
	}
	else
	{
		for (auto& data : aJson["Animations"])
		{
			AddAnimation(AssetManager::GetAsset<Animation>(data["Path"].asString()), myBoneIndex, data["Value"].asFloat());
		}
	}
}

Json::Value BlendSpace::CreateJson() const
{
	Json::Value result;
	result["Name"] = myName;
	result["BoneIndex"] = myBoneIndex;
	result["Animations"] = Json::arrayValue;
	unsigned index = 0;
	for (auto& data : myAnimations)
	{
		auto& json = result["Animations"][index];
		json["Path"] = data.animation->GetPath();
		json["Value"] = data.blendValue;
		++index;
	}
	return result;
}

void BlendSpace::ValidateUsingNamespace(const Skeleton* aSkeleton)
{
	myFlags[eIsUsingNamespace] = false;
	for (auto& data : myAnimations)
	{
		data.animation->ValidateUsingNamespace(aSkeleton);
		if (data.animation->IsUsingNamespace())
		{
			myFlags[eIsUsingNamespace] = true;
		}
	}
}

void BlendSpace::AddInternal()
{
	if (myHasMatchingFPS)
	{
		UpdateMatchingFPS();
	}
	const auto& data = myAnimations.back();

	if (mySkeleton)
	{
		data.animation->Init(*myBoneCache, mySkeleton);
		data.animation->ValidateUsingNamespace(mySkeleton);
		if (data.animation->IsUsingNamespace())
		{
			myFlags[eIsUsingNamespace] = true;
		}
	}
	
	if (myLongestAnimation)
	{
		if (data.animation->GetData().length > myLongestAnimation->GetData().length)
		{
			myLongestAnimation = data.animation.get();
		}
	}
	else
	{
		myLongestAnimation = data.animation.get();
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
			if (myFlags[eIsReversing])
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

void BlendSpace::GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const
{
	assert(!"Not Implemented!");
	outTransforms; anIndex; aFrame; aParentTransform;
}

void BlendSpace::GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const
{
	assert(!"Not Implemented!");
	outTransforms; anIndex; aCurrentFrame; anInterpolationFrame; anInterpolationValue; aParentTransform;
}
