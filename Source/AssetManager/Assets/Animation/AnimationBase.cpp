#include "AssetManager.pch.h"
#include "AnimationBase.h"

Crimson::Matrix4x4f AnimationTransform::GetAsMatrix() const
{
	Crimson::Matrix4x4f result = rotation.GetAsMatrix3x3();
	result *= Crimson::Matrix4x4f::CreateTranslationMatrix(position);
	return result;
}

AnimationTransform AnimationTransform::Interpolate(const AnimationTransform& aFrom, const AnimationTransform& aTo, float aPercentage)
{
	return AnimationTransform{ Lerp(aFrom.position, aTo.position, aPercentage), Nlerp(aFrom.rotation, aTo.rotation, aPercentage) };
}

AnimationFrame::AnimationFrame(const TGA::FBX::Animation::Frame& aFrame) : globalTransformMatrices(), localTransformMatrices(), socketTransforms(), triggeredEvents(aFrame.TriggeredEvents)
{
	globalTransformMatrices.reserve(aFrame.GlobalTransforms.size());
	globalTransforms.reserve(aFrame.GlobalTransforms.size());
	for (auto& [key, value] : aFrame.GlobalTransforms)
	{
		const auto& matrix = ConvertMatrix(value);
		globalTransformMatrices.emplace(key, matrix);
		globalTransforms.emplace(key, AnimationTransform{ matrix.GetTranslation(), Crimson::QuatF(matrix).GetNormalized()});
	}

	localTransformMatrices.reserve(aFrame.LocalTransforms.size());
	for (auto& [key, value] : aFrame.LocalTransforms)
	{
		localTransformMatrices.emplace(key, ConvertMatrix(value));
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
	myTargetFrameDelta(0.f),
	myAnimationTimer(0.f),
	myInterpolationTimer(0.f),
	myType(aType),
	myIsPlaying(false),
	myIsLooping(false),
	myIsPlayingInReverse(false)
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

void AnimationBase::LoadFromJson(const Json::Value& aJson)
{
	assert(!"Not Implemented!");
	aJson;
}

Json::Value AnimationBase::ToJson() const
{
	assert(!"Not Implemented!");
	return Json::Value();
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
