#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"
#include "GraphicsEngine/GraphicsEngineDefines.h"
#include "Math/Quaternion.hpp"
#include "Math/Transform.h"
#include <bitset>

class Skeleton;

struct AnimationTransform
{
	Crimson::Vector3f position;
	Crimson::QuatF rotation;

	AnimationTransform() = default;
	AnimationTransform(const Crimson::Matrix4x4f& aMatrix);
	AnimationTransform(const Crimson::Vector3f& aPosition, const Crimson::QuatF& aRotation);

	void Serialize(std::ostream& aStream) const;
	void Deserialize(std::istream& aStream);

	Crimson::Matrix4x4f GetAsMatrix() const;
	static AnimationTransform Interpolate(const AnimationTransform& aFrom, const AnimationTransform& aTo, float aPercentage);

	inline bool operator==(const AnimationTransform& aTranform) const
	{
		return position == aTranform.position && rotation == aTranform.rotation;
	}
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
	AnimationFrame(std::istream& aStream);

	void Serialize(std::ostream& aStream) const;
};

struct AnimationData
{
	std::vector<AnimationFrame> frames;
	std::vector<std::string> eventNames;
	std::string name;
	double duration;
	float framesPerSecond;
	float frameDelta;
	unsigned length;

	AnimationData(const TGA::FBX::Animation& anAnimation);
	AnimationData(std::istream& aStream);

	void Serialize(std::ostream& aStream) const;
};

class AnimationBase
{
protected:
	friend class SkeletonEditor;
	typedef std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> BoneCache;
public:
	enum class AnimationType : unsigned char
	{
		Invalid,
		Animation,
		AnimationLayer,
		BlendSpace
	};

	AnimationBase(AnimationType aType);
	AnimationBase(const AnimationBase& anAnimation);
	AnimationBase(AnimationBase&& anAnimation) noexcept;
	virtual ~AnimationBase() = default;

	// Will be called when added to a Component
	virtual void Init(BoneCache& aBoneCache, const Skeleton* aSkeleton);

	virtual void Init(const Json::Value& aJson);

	/// <param name="aTargetFPS">: Will interpolate frames to match the specified FPS. Will use default FPS of Animation if 0</param>
	/// <param name="aIsLooping">: Stops on the last frame if not looping</param>
	/// <param name="aPlayInReverse">: Used to choose which way to iterate through frames</param>
	void SetPlaySettings(float aTargetFPS = 0.f, bool aIsLooping = false, bool aPlayInReverse = false);

	// Returns true while playing
	virtual bool Update() = 0;

	/// <param name="aTimeSinceLastUpdate">The time since last this function was called, if called every frame it is the deltatime</param>
	/// <returns>True if the animation altered the transform</returns>
	virtual bool UpdateRootMotion(float aTimeSinceLastUpdate) = 0;

	/// <param name="aPercentage">The amount of the total motion from the current frame to get, from 0.f to 1.f</param>
	/// <returns>The interpolated motion</returns>
	virtual AnimationTransform GetRootMotion(float aPercentage) = 0;

	void StartAnimation();
	void StopAnimation();
	bool IsPlaying() const;

	void EnableRootMotion(Transform& aTransformToApplyMotionTo);
	void DisableRootMotion();

	void ResetTimer();

	void SetAdditiveAnimation(bool aState);

	void SetTargetFPS(float aFPS);
	void SetTargetFrameDelta(float aFrameDelta);
	virtual float GetFrameDelta() const = 0;

	void SetIsLooping(bool aShouldLoop);
	void ToogleLooping();
	bool IsLooping() const;
	virtual bool IsEndOfLoop() const = 0;

	virtual void SetIsPlayingInReverse(bool aShouldPlayBackwards);
	bool IsPlayingInReverse() const;

	virtual const std::string& GetPath() const = 0;
	AnimationType GetType() const;
	virtual unsigned GetStartBoneIndex() const = 0;

	virtual void SetToFirstFrame() = 0;
	virtual void SetToLastFrame() = 0;

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	virtual bool NextFrame() = 0;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	virtual bool PreviousFrame() = 0;

	virtual void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const = 0;
	virtual void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const = 0;

	virtual std::unordered_map<std::string, AnimationTransform> GetAdditiveTransforms() const = 0;

	virtual std::unordered_map<std::string, AnimationTransform> GetFrameTransforms() const = 0;
	virtual std::unordered_map<std::string, AnimationTransform> GetFrameTransforms(float anInterpolationValue) const = 0;

	virtual bool IsValid() const = 0;
	virtual bool HasData() const = 0;

	virtual bool IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage = nullptr) const = 0;
	bool IsUsingNamespace() const ;

	virtual std::shared_ptr<AnimationBase> GetAsSharedPtr() const = 0;

	virtual Json::Value ToJson() const;

	//virtual void Serialize(std::ostream& aStream) const = 0;
	//virtual void Deserialize(std::istream& aStream) = 0;

	virtual void ValidateUsingNamespace(const Skeleton* aSkeleton) = 0;
protected:
	const Skeleton* mySkeleton;
	BoneCache* myBoneCache;
	Transform* myRootMotionTransform;
	float myTargetFrameDelta;
	float myAnimationTimer;
	float myInterpolationTimer;
	AnimationType myType;

	enum Flags
	{
		eIsPlaying,
		eIsLooping,
		eIsReversing,
		eIsUsingNamespace,
		eIsAdditive,
		eFlagCount
	};
	std::bitset<eFlagCount> myFlags;
};

std::shared_ptr<AnimationBase> LoadAnimationFromJson(const std::string& aPath, const Json::Value& aJson);