#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"

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

class Animation
{
public:
	Animation();
	Animation(AnimationData& someData);
	~Animation() = default;

	float GetFPS() const;
	float GetFrameDelta() const;
	const AnimationFrame& GetFrame(unsigned int anIndex) const;
	const std::string& GetName() const;
	const std::string& GetPath() const;

	// Returns true if next frame is first frame
	bool GetNextIndex(unsigned int& outIndex) const;

	bool HasData() const;
	const AnimationData& GetData() const;

private:
	AnimationData* myData;
};