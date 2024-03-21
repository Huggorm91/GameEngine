#include "AssetManager.pch.h"
#include "AnimationBase.h"
#include "AssetManager.h"

AnimationTransform::AnimationTransform(const Crimson::Matrix4x4f& aMatrix) :
	position(aMatrix.GetTranslation()), 
	rotation(Crimson::QuatF(aMatrix).GetNormalized())
{}

AnimationTransform::AnimationTransform(const Crimson::Vector3f& aPosition, const Crimson::QuatF& aRotation) :
	position(aPosition), 
	rotation(aRotation)
{}

void AnimationTransform::Serialize(std::ostream& aStream) const
{
	position.Serialize(aStream);
	rotation.Serialize(aStream);
}

void AnimationTransform::Deserialize(std::istream& aStream)
{
	position.Deserialize(aStream);
	rotation.Deserialize(aStream);
}

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

AnimationFrame::AnimationFrame(const TGA::FBX::Animation::Frame& aFrame) : triggeredEvents(aFrame.TriggeredEvents)
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

AnimationFrame::AnimationFrame(std::istream& aStream)
{
	// Global Transforms
	unsigned globalTransformCount;
	aStream.read(reinterpret_cast<char*>(&globalTransformCount), sizeof(globalTransformCount));
	globalTransforms.reserve(globalTransformCount);
	for (unsigned i = 0; i < globalTransformCount; i++)
	{
		std::string name;
		std::getline(aStream, name, '\0');
		AnimationTransform transform;
		transform.Deserialize(aStream);

		globalTransforms.emplace(name, transform);
		globalTransformMatrices.emplace(name, transform.GetAsMatrix());
	}

	// Local Transforms
	unsigned localTransformCount;
	aStream.read(reinterpret_cast<char*>(&localTransformCount), sizeof(localTransformCount));
	localTransforms.reserve(localTransformCount);
	for (unsigned i = 0; i < localTransformCount; i++)
	{
		std::string name;
		std::getline(aStream, name, '\0');
		AnimationTransform transform;
		transform.Deserialize(aStream);

		localTransforms.emplace(name, transform);
		localTransformMatrices.emplace(name, transform.GetAsMatrix());
	}

	// Socket Transforms
	unsigned socketTransformCount;
	aStream.read(reinterpret_cast<char*>(&socketTransformCount), sizeof(socketTransformCount));
	socketTransforms.reserve(socketTransformCount);
	for (unsigned i = 0; i < socketTransformCount; i++)
	{
		std::string name;
		std::getline(aStream, name, '\0');
		Crimson::Matrix4x4f matrix;
		matrix.Deserialize(aStream);

		socketTransforms.emplace(std::move(name), std::move(matrix));
	}

	// Triggered Events
	unsigned triggeredEventsCount;
	aStream.read(reinterpret_cast<char*>(&triggeredEventsCount), sizeof(triggeredEventsCount));
	triggeredEvents.reserve(triggeredEventsCount);
	for (unsigned i = 0; i < triggeredEventsCount; i++)
	{
		std::string name;
		std::getline(aStream, name, '\0');
		bool state;
		aStream.read(reinterpret_cast<char*>(&state), sizeof(state));

		triggeredEvents.emplace(std::move(name), std::move(state));
	}
}

void AnimationFrame::Serialize(std::ostream& aStream) const
{
	// Global Transforms
	unsigned globalTransformCount = static_cast<unsigned>(globalTransforms.size());
	aStream.write(reinterpret_cast<const char*>(&globalTransformCount), sizeof(globalTransformCount));
	for (auto& [name, transform] : globalTransforms)
	{
		aStream.write(name.c_str(), name.size() + 1);
		transform.Serialize(aStream);
	}

	// Local Transforms
	unsigned localTransformCount = static_cast<unsigned>(localTransforms.size());
	aStream.write(reinterpret_cast<const char*>(&localTransformCount), sizeof(localTransformCount));
	for (auto& [name, transform] : localTransforms)
	{
		aStream.write(name.c_str(), name.size() + 1);
		transform.Serialize(aStream);
	}

	// Socket Transforms
	unsigned socketTransformCount = static_cast<unsigned>(socketTransforms.size());
	aStream.write(reinterpret_cast<const char*>(&socketTransformCount), sizeof(socketTransformCount));
	for (auto& [name, matrix] : socketTransforms)
	{
		aStream.write(name.c_str(), name.size() + 1);
		matrix.Serialize(aStream);
	}

	// Triggered Events
	unsigned triggeredEventsCount = static_cast<unsigned>(triggeredEvents.size());
	aStream.write(reinterpret_cast<const char*>(&triggeredEventsCount), sizeof(triggeredEventsCount));
	for (auto& [name, state] : triggeredEvents)
	{
		aStream.write(name.c_str(), name.size() + 1);
		aStream.write(reinterpret_cast<const char*>(&state), sizeof(state));
	}
}

AnimationData::AnimationData(const TGA::FBX::Animation& anAnimation) :
	eventNames(anAnimation.EventNames),
	name(anAnimation.Name),
	duration(anAnimation.Duration),
	framesPerSecond(anAnimation.FramesPerSecond),
	frameDelta(1.f / framesPerSecond), length(anAnimation.Length)
{
	for (auto& frame : anAnimation.Frames)
	{
		frames.emplace_back(frame);
	}
}

AnimationData::AnimationData(std::istream& aStream) :
	duration(0.),
	framesPerSecond(0.f),
	frameDelta(0.f),
	length(0u)
{
	// Frames
	unsigned frameCount;
	aStream.read(reinterpret_cast<char*>(&frameCount), sizeof(frameCount));
	for (unsigned i = 0; i < frameCount; i++)
	{
		frames.emplace_back(aStream);
	}

	// Event Names
	unsigned eventCount;
	aStream.read(reinterpret_cast<char*>(&eventCount), sizeof(eventCount));
	for (unsigned i = 0; i < eventCount; i++)
	{
		std::string eventName;
		std::getline(aStream, eventName, '\0');
		eventNames.emplace_back(std::move(eventName));
	}

	std::getline(aStream, name, '\0');
	aStream.read(reinterpret_cast<char*>(&duration), sizeof(duration));
	aStream.read(reinterpret_cast<char*>(&framesPerSecond), sizeof(framesPerSecond));
	aStream.read(reinterpret_cast<char*>(&frameDelta), sizeof(frameDelta));
	aStream.read(reinterpret_cast<char*>(&length), sizeof(length));
}

void AnimationData::Serialize(std::ostream& aStream) const
{
	// Frames
	unsigned frameCount = static_cast<unsigned>(frames.size());
	aStream.write(reinterpret_cast<const char*>(&frameCount), sizeof(frameCount));
	for (auto& frame : frames)
	{
		frame.Serialize(aStream);
	}

	// Event Names
	unsigned eventCount = static_cast<unsigned>(eventNames.size());
	aStream.write(reinterpret_cast<const char*>(&eventCount), sizeof(eventCount));
	for (auto& eventName : eventNames)
	{
		aStream.write(eventName.c_str(), eventName.size() + 1);
	}

	aStream.write(name.c_str(), name.size() + 1);
	aStream.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
	aStream.write(reinterpret_cast<const char*>(&framesPerSecond), sizeof(framesPerSecond));
	aStream.write(reinterpret_cast<const char*>(&frameDelta), sizeof(frameDelta));
	aStream.write(reinterpret_cast<const char*>(&length), sizeof(length));
}

AnimationBase::AnimationBase(AnimationType aType) :
	mySkeleton(nullptr),
	myBoneCache(nullptr),
	myRootMotionTransform(nullptr),
	myTargetFrameDelta(0.f),
	myAnimationTimer(0.f),
	myInterpolationTimer(0.f),
	myType(aType)
{}

AnimationBase::AnimationBase(const AnimationBase& anAnimation) :
	mySkeleton(anAnimation.mySkeleton),
	myBoneCache(anAnimation.myBoneCache),
	myRootMotionTransform(anAnimation.myRootMotionTransform),
	myTargetFrameDelta(anAnimation.myTargetFrameDelta),
	myAnimationTimer(anAnimation.myAnimationTimer),
	myInterpolationTimer(anAnimation.myInterpolationTimer),
	myType(anAnimation.myType),
	myFlags(anAnimation.myFlags)
{}

AnimationBase::AnimationBase(AnimationBase&& anAnimation) noexcept :
	mySkeleton(anAnimation.mySkeleton),
	myBoneCache(anAnimation.myBoneCache),
	myRootMotionTransform(anAnimation.myRootMotionTransform),
	myTargetFrameDelta(anAnimation.myTargetFrameDelta),
	myAnimationTimer(anAnimation.myAnimationTimer),
	myInterpolationTimer(anAnimation.myInterpolationTimer),
	myType(anAnimation.myType),
	myFlags(anAnimation.myFlags)
{}

void AnimationBase::Init(BoneCache& aBoneCache, const Skeleton* aSkeleton)
{
	myBoneCache = &aBoneCache;
	mySkeleton = aSkeleton;
	if (IsValid())
	{
		ValidateUsingNamespace(aSkeleton);
		UpdateBoneCache(mySkeleton, *myBoneCache);
	}
}

void AnimationBase::Init(const Json::Value& aJson)
{
	myTargetFrameDelta = aJson["TargetFrameDelta"].asFloat();
	myType = static_cast<AnimationType>(aJson["Type"].asInt());

	myFlags[eIsPlaying] = aJson["IsPlaying"].asBool();
	myFlags[eIsLooping] = aJson["IsLooping"].asBool();
	myFlags[eIsReversing] = aJson["IsPlayingInReverse"].asBool();
	myFlags[eIsUsingNamespace] = aJson["UsesNamespace"].asBool();
}

void AnimationBase::SetPlaySettings(float aTargetFPS, bool aIsLooping, bool aPlayInReverse)
{
	myTargetFrameDelta = 1.f / aTargetFPS;
	myFlags[eIsLooping] = aIsLooping;
	myFlags[eIsReversing] = aPlayInReverse;
}

void AnimationBase::StartAnimation()
{
	myFlags[eIsPlaying] = true;
}

void AnimationBase::StopAnimation()
{
	myFlags[eIsPlaying] = false;
}

bool AnimationBase::IsPlaying() const
{
	return myFlags[eIsPlaying];
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
	myFlags[eIsLooping] = aShouldLoop;
}

void AnimationBase::ToogleLooping()
{
	myFlags.flip(eIsLooping);
}

bool AnimationBase::IsLooping() const
{
	return myFlags[eIsLooping];
}

void AnimationBase::SetIsPlayingInReverse(bool aShouldPlayBackwards)
{
	myFlags[eIsReversing] = aShouldPlayBackwards;
}

bool AnimationBase::IsPlayingInReverse() const
{
	return myFlags[eIsReversing];
}

AnimationBase::AnimationType AnimationBase::GetType() const
{
	return myType;
}

bool AnimationBase::IsValid() const
{
	return mySkeleton && myBoneCache;
}

bool AnimationBase::IsUsingNamespace() const
{
	return myFlags[eIsUsingNamespace];
}

Json::Value AnimationBase::ToJson() const
{
	Json::Value result;
	result["TargetFrameDelta"] = myTargetFrameDelta;
	result["Type"] = static_cast<int>(myType);
	result["IsPlaying"] = myFlags[eIsPlaying];
	result["IsLooping"] = myFlags[eIsLooping];
	result["IsPlayingInReverse"] = myFlags[eIsReversing];
	result["UsesNamespace"] = myFlags[eIsUsingNamespace];
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
