#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"
#include "GraphicsEngine/GraphicsEngineDefines.h"
#include "Math/Quaternion.hpp"

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
	virtual ~AnimationBase() = default;

	virtual const std::string& GetName() const = 0;

	virtual float GetFPS() const = 0;
	virtual float GetFrameDelta() const = 0;

	virtual void SetToFirstFrame() = 0;
	virtual void SetToLastFrame() = 0;

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	virtual bool NextFrame() = 0;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	virtual bool PreviousFrame() = 0;

	virtual void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const = 0;
	virtual void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue, bool anInterpolatePreviousFrame = false) const = 0;

	virtual bool IsValid() const = 0;

	//virtual void Serialize(std::ostream& aStream) const = 0;
	//virtual void Deserialize(std::istream& aStream) = 0;
};

