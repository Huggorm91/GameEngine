#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"
#include "GraphicsEngine/GraphicsEngineDefines.h"
#include "Math/Quaternion.hpp"
#include "Math/Transform.h"

class Skeleton;

struct AnimationTransform
{
	Crimson::Vector3f position;
	Crimson::QuatF rotation;

	Crimson::Matrix4x4f GetAsMatrix() const;
	static AnimationTransform Interpolate(const AnimationTransform& aFrom, const AnimationTransform& aTo, float aPercentage);
};

struct AnimationFrame
{
	std::unordered_map<std::string, Crimson::Matrix4x4f> globalTransformMatrices;
	std::unordered_map<std::string, Crimson::Matrix4x4f> localTransformMatrices;
	std::unordered_map<std::string, AnimationTransform> globalTransforms;
	std::unordered_map<std::string, AnimationTransform> localTransforms;
	std::unordered_map<std::string, Crimson::Matrix4x4f> socketTransforms;
	std::unordered_map<std::string, bool> triggeredEvents;

	AnimationFrame(const TGA::FBX::Animation::Frame& aFrame);
};

struct AnimationData
{
	std::vector<AnimationFrame> frames;
	std::vector<std::string> eventNames;
	std::string name;
	double duration;
	float framesPerSecond;
	float frameDelta;
	unsigned int length;

	AnimationData(const TGA::FBX::Animation& anAnimation);
};

class AnimationBase
{
protected:
	typedef std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> BoneCache;
public:
	enum class AnimationType : char
	{
		Invalid,
		Animation,
		AnimationLayer,
		BlendSpace
	};
	AnimationBase(AnimationType aType);
	virtual ~AnimationBase() = default;

	// Will be called when added to a Component
	virtual void Init(BoneCache& aBoneCache, const Skeleton* aSkeleton);

	virtual void LoadFromJson(const Json::Value& aJson);
	virtual Json::Value ToJson() const;

	/// <param name="aTargetFPS">: Will interpolate frames to match the specified FPS. Will use default FPS of Animation if 0</param>
	/// <param name="aIsLooping">: Stops on the last frame if not looping</param>
	/// <param name="aPlayInReverse">: Used to choose which way to iterate through frames</param>
	void SetPlaySettings(float aTargetFPS = 0.f, bool aIsLooping = false, bool aPlayInReverse = false);

	// Returns true while playing
	virtual bool Update() = 0;

	void StartAnimation();
	void StopAnimation();
	bool IsPlaying() const;

	void EnableRootMotion(Transform& aTransformToApplyMotionTo);
	void DisableRootMotion();

	void ResetTimer();

	void SetTargetFPS(float anFPS);
	float GetTargetFrameDelta() const;

	void SetIsLooping(bool aShouldLoop);
	void ToogleLooping();
	bool IsLooping() const;

	void SetIsPlayingInReverse(bool aShouldPlayBackwards);
	bool IsPlayingInReverse() const;

	virtual const std::string& GetName() const = 0;
	AnimationType GetType() const;

	virtual void SetToFirstFrame() = 0;
	virtual void SetToLastFrame() = 0;

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	virtual bool NextFrame() = 0;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	virtual bool PreviousFrame() = 0;

	virtual void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const = 0;
	virtual void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const = 0;

	virtual bool IsValid() const = 0;
	virtual bool HasData() const = 0;

	virtual bool IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage = nullptr) const = 0;

	virtual std::shared_ptr<AnimationBase> GetAsSharedPtr() const = 0;

	//virtual void Serialize(std::ostream& aStream) const = 0;
	//virtual void Deserialize(std::istream& aStream) = 0;

protected:
	const Skeleton* mySkeleton;
	BoneCache* myBoneCache;
	Transform* myRootMotionTransform;
	float myTargetFrameDelta;
	float myAnimationTimer;
	float myInterpolationTimer;
	AnimationType myType;
	bool myIsPlaying;
	bool myIsLooping;
	bool myIsPlayingInReverse;
};

