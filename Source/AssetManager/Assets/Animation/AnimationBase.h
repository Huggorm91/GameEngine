#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"
#include "GraphicsEngine/GraphicsEngineDefines.h"

class Skeleton;

struct AnimationFrame
{
	std::unordered_map<std::string, Crimson::Matrix4x4f> myGlobalTransforms;
	std::unordered_map<std::string, Crimson::Matrix4x4f> myLocalTransforms;
	std::unordered_map<std::string, Crimson::Matrix4x4f> mySocketTransforms;
	std::unordered_map<std::string, bool> myTriggeredEvents;

	AnimationFrame(const TGA::FBX::Animation::Frame& aFrame);
};

struct AnimationData
{
	std::vector<AnimationFrame> myFrames;
	std::vector<std::string> myEventNames;
	std::string myName;
	double myDuration;
	const std::string* myPath;
	float myFramesPerSecond;
	float myFrameDelta;
	unsigned int myLength;

	AnimationData(const TGA::FBX::Animation& anAnimation);
};

class AnimationBase
{
public:
	virtual ~AnimationBase() = default;

	virtual const std::string& GetName() const = 0;
	virtual const std::string& GetPath() const = 0;

	virtual float GetFPS() const = 0;
	virtual float GetFrameDelta() const = 0;

	virtual void SetToFirstFrame() = 0;
	virtual void SetToLastFrame() = 0;

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	virtual bool NextFrame() = 0;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	virtual bool PreviousFrame() = 0;

	virtual void UpdateBoneCache(const Skeleton* aSkeleton, std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& outBones) const = 0;

	virtual bool IsValid() const = 0;

	//virtual void Serialize(std::ostream& aStream) const = 0;
	//virtual void Deserialize(std::istream& aStream) = 0;
};

