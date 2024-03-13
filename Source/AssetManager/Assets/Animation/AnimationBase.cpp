#include "AssetManager.pch.h"
#include "AnimationBase.h"
#include "AssetManager.h"

AnimationTransform::AnimationTransform(const Crimson::Matrix4x4f& aMatrix) :position(aMatrix.GetTranslation()), rotation(Crimson::QuatF(aMatrix).GetNormalized())
{}

AnimationTransform::AnimationTransform(const Crimson::Vector3f& aPosition, const Crimson::QuatF& aRotation) :position(aPosition), rotation(aRotation)
{}

Crimson::Matrix4x4f AnimationTransform::GetAsMatrix() const
{
	Crimson::Matrix4x4f result = rotation.GetAsMatrix3x3();
	result *= Crimson::Matrix4x4f::CreateTranslationMatrix(position);
	return result;
}

AnimationTransform AnimationTransform::Interpolate(const AnimationTransform& aFrom, const AnimationTransform& aTo, float aPercentage)
{
	return AnimationTransform(Lerp(aFrom.position, aTo.position, aPercentage), Nlerp(aFrom.rotation, aTo.rotation, aPercentage));
}

AnimationFrame::AnimationFrame(const TGA::FBX::Animation::Frame& aFrame) : globalTransformMatrices(), localTransformMatrices(), socketTransforms(), triggeredEvents(aFrame.TriggeredEvents)
{
	globalTransformMatrices.reserve(aFrame.GlobalTransforms.size());
	globalTransforms.reserve(aFrame.GlobalTransforms.size());
	for (auto& [key, value] : aFrame.GlobalTransforms)
	{
		const auto& matrix = ConvertMatrix(value);
		globalTransformMatrices.emplace(key, matrix);
		globalTransforms.emplace(key, matrix);
	}

	localTransformMatrices.reserve(aFrame.LocalTransforms.size());
	for (auto& [key, value] : aFrame.LocalTransforms)
	{
		const auto& matrix = ConvertMatrix(value);
		localTransformMatrices.emplace(key, matrix);
		localTransforms.emplace(key, matrix);
	}

	socketTransforms.reserve(aFrame.SocketTransforms.size());
	for (auto& [key, value] : aFrame.SocketTransforms)
	{
		socketTransforms.emplace(key, ConvertMatrix(value));
	}
}

AnimationData::AnimationData(const TGA::FBX::Animation& anAnimation) : frames(), eventNames(anAnimation.EventNames), name(anAnimation.Name), duration(anAnimation.Duration), framesPerSecond(anAnimation.FramesPerSecond),
frameDelta(1.f / framesPerSecond), length(anAnimation.Length)
{
	for (auto& frame : anAnimation.Frames)
	{
		frames.emplace_back(frame);
	}
}

AnimationBase::AnimationBase(AnimationType aType) :
	mySkeleton(nullptr),
	myBoneCache(nullptr),
	myRootMotionTransform(nullptr),
	myTargetFrameDelta(0.f),
	myAnimationTimer(0.f),
	myInterpolationTimer(0.f),
	myType(aType),
	myIsPlaying(false),
	myIsLooping(false),
	myIsPlayingInReverse(false)
{}

AnimationBase::AnimationBase(const AnimationBase& anAnimation): 
	mySkeleton(anAnimation.mySkeleton),
	myBoneCache(anAnimation.myBoneCache),
	myRootMotionTransform(anAnimation.myRootMotionTransform),
	myTargetFrameDelta(anAnimation.myTargetFrameDelta),
	myAnimationTimer(anAnimation.myAnimationTimer),
	myInterpolationTimer(anAnimation.myInterpolationTimer),
	myType(anAnimation.myType),
	myIsPlaying(anAnimation.myIsPlaying),
	myIsLooping(anAnimation.myIsLooping),
	myIsPlayingInReverse(anAnimation.myIsPlayingInReverse)
{}

AnimationBase::AnimationBase(AnimationBase && anAnimation) noexcept :
	mySkeleton(anAnimation.mySkeleton),
	myBoneCache(anAnimation.myBoneCache),
	myRootMotionTransform(anAnimation.myRootMotionTransform),
	myTargetFrameDelta(anAnimation.myTargetFrameDelta),
	myAnimationTimer(anAnimation.myAnimationTimer),
	myInterpolationTimer(anAnimation.myInterpolationTimer),
	myType(anAnimation.myType),
	myIsPlaying(anAnimation.myIsPlaying),
	myIsLooping(anAnimation.myIsLooping),
	myIsPlayingInReverse(anAnimation.myIsPlayingInReverse)
{}

void AnimationBase::Init(BoneCache& aBoneCache, const Skeleton* aSkeleton)
{
	myBoneCache = &aBoneCache;
	mySkeleton = aSkeleton;
	if (IsValid())
	{
		UpdateBoneCache(mySkeleton, *myBoneCache);
	}
}

void AnimationBase::Init(const Json::Value& aJson)
{
	myTargetFrameDelta= aJson["TargetFrameDelta"].asFloat();
	myType= static_cast<AnimationType>(aJson["Type"].asInt());
	myIsPlaying= aJson["IsPlaying"].asBool();
	myIsLooping= aJson["IsLooping"].asBool();
	myIsPlayingInReverse= aJson["IsPlayingInReverse"].asBool();
}

void AnimationBase::SetPlaySettings(float aTargetFPS, bool aIsLooping, bool aPlayInReverse)
{
	myTargetFrameDelta = 1.f / aTargetFPS;
	myIsLooping = aIsLooping;
	myIsPlayingInReverse = aPlayInReverse;
}

void AnimationBase::StartAnimation()
{
	myIsPlaying = true;
}

void AnimationBase::StopAnimation()
{
	myIsPlaying = false;
}

bool AnimationBase::IsPlaying() const
{
	return myIsPlaying;
}

void AnimationBase::EnableRootMotion(Transform& aTransformToApplyMotionTo)
{
	myRootMotionTransform = &aTransformToApplyMotionTo;
}

void AnimationBase::DisableRootMotion()
{
	myRootMotionTransform = nullptr;
}

void AnimationBase::ResetTimer()
{
	myAnimationTimer = 0.f;
	myInterpolationTimer = 0.f;
}

void AnimationBase::SetTargetFPS(float anFPS)
{
	myTargetFrameDelta = 1.f / anFPS;
}

float AnimationBase::GetTargetFrameDelta() const
{
	return myTargetFrameDelta;
}

void AnimationBase::SetIsLooping(bool aShouldLoop)
{
	myIsLooping = aShouldLoop;
}

void AnimationBase::ToogleLooping()
{
	myIsLooping = !myIsLooping;
}

bool AnimationBase::IsLooping() const
{
	return myIsLooping;
}

void AnimationBase::SetIsPlayingInReverse(bool aShouldPlayBackwards)
{
	myIsPlayingInReverse = aShouldPlayBackwards;
}

bool AnimationBase::IsPlayingInReverse() const
{
	return myIsPlayingInReverse;
}

AnimationBase::AnimationType AnimationBase::GetType() const
{
	return myType;
}

bool AnimationBase::IsValid() const
{
	return mySkeleton && myBoneCache;
}

Json::Value AnimationBase::ToJson() const
{
	Json::Value result;
	result["TargetFrameDelta"] = myTargetFrameDelta;
	result["Type"] = static_cast<int>(myType);
	result["IsPlaying"] = myIsPlaying;
	result["IsLooping"] = myIsLooping;
	result["IsPlayingInReverse"] = myIsPlayingInReverse;
	return result;
}

std::shared_ptr<AnimationBase> LoadAnimationFromJson(const std::string& aPath, const Json::Value& aJson)
{
	AnimationBase::AnimationType type = static_cast<AnimationBase::AnimationType>(aJson["Type"].asInt());
	switch (type)
	{
	case AnimationBase::AnimationType::Animation:
	{
		auto ptr = std::make_shared<Animation>(AssetManager::GetAsset<Animation>(aPath));
		ptr->Init(aJson);
		return ptr;
	}
	case AnimationBase::AnimationType::AnimationLayer:
	{
		auto ptr = std::make_shared<AnimationLayer>(AssetManager::GetAsset<Animation>(aPath));
		ptr->Init(aJson);
		return ptr;
	}
	case AnimationBase::AnimationType::BlendSpace:
	{
		auto ptr = std::make_shared<BlendSpace>(AssetManager::GetAsset<BlendSpace>(aPath));
		ptr->Init(aJson);
		return ptr;
	}
	default:
		break;
	}
	return std::shared_ptr<AnimationBase>();
}
