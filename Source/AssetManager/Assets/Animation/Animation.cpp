#include "AssetManager.pch.h"
#include "Animation.h"

AnimationFrame::AnimationFrame(const TGA::FBX::Animation::Frame& aFrame) : myGlobalTransforms(), myLocalTransforms(), mySocketTransforms(), myTriggeredEvents(aFrame.TriggeredEvents)
{
	for (auto& [key, value] : aFrame.GlobalTransforms)
	{
		myGlobalTransforms.emplace(key, ConvertMatrix(value));
	}

	for (auto& [key, value] : aFrame.LocalTransforms)
	{
		myLocalTransforms.emplace(key, ConvertMatrix(value));
	}

	for (auto& [key, value] : aFrame.SocketTransforms)
	{
		mySocketTransforms.emplace(key, ConvertMatrix(value));
	}
}

AnimationData::AnimationData(const TGA::FBX::Animation& anAnimation) : myFrames(), myEventNames(anAnimation.EventNames), myName(anAnimation.Name), myDuration(anAnimation.Duration), myFramesPerSecond(anAnimation.FramesPerSecond),
myFrameDelta(1.f / myFramesPerSecond), myLength(anAnimation.Length), myPath(nullptr)
{
	for (auto& frame : anAnimation.Frames)
	{
		myFrames.emplace_back(frame);
	}
}

Animation::Animation() : myData(nullptr)
{}

Animation::Animation(AnimationData& someData) : myData(&someData)
{}

const std::string& Animation::GetName() const
{
	return myData->myName;
}

const std::string& Animation::GetPath() const
{
	return *myData->myPath;
}

float Animation::GetFPS() const
{
	return myData->myFramesPerSecond;
}

float Animation::GetFrameDelta() const
{
	return myData->myFrameDelta;
}

unsigned Animation::GetFrameCount() const
{
	return myData->myLength;
}

const AnimationFrame& Animation::GetFrame(unsigned int anIndex) const
{
	return myData->myFrames[anIndex];
}

unsigned Animation::GetLastFrameIndex() const
{
	return myData->myLength - 1;
}

bool Animation::GetNextIndex(unsigned int& outIndex) const
{
	if (++outIndex >= myData->myLength)
	{
		outIndex = 1;
		return true;
	}
	return false;
}

bool Animation::HasData() const
{
	return myData != nullptr;
}

const AnimationData& Animation::GetData() const
{
	return *myData;
}