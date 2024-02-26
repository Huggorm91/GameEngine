#include "AssetManager.pch.h"
#include "Animation.h"
#include "Skeleton.h"

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

Animation::Animation() : myData(nullptr), myCurrentFrame(1)
{}

Animation::Animation(AnimationData& someData) : myData(&someData), myCurrentFrame(1)
{}

bool Animation::operator==(const Animation& anAnimation) const
{
	return myData == anAnimation.myData;
}

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

void Animation::SetToFirstFrame()
{
	myCurrentFrame = 1;
}

void Animation::SetToLastFrame()
{
	myCurrentFrame = myData->myLength - 1;
}

void Animation::SetFrameIndex(unsigned anIndex)
{
	myCurrentFrame = Crimson::Min(anIndex, GetLastFrameIndex());
}

bool Animation::NextFrame()
{
	if (++myCurrentFrame == myData->myLength)
	{
		SetToFirstFrame();
	}
	if (myCurrentFrame == myData->myLength -1)
	{
		return false;
	}
	return true;
}

bool Animation::PreviousFrame()
{
	if (--myCurrentFrame == 0)
	{
		SetToLastFrame();
	}
	if (myCurrentFrame == 1)
	{
		return false;
	}
	return true;
}

void Animation::UpdateBoneCache(const Skeleton* aSkeleton, std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& outBones) const
{
	UpdateBoneCacheInternal(aSkeleton, outBones, 0u);
}

const AnimationFrame& Animation::GetFrame(unsigned int anIndex) const
{
	return myData->myFrames[anIndex];
}

const AnimationFrame& Animation::GetCurrentFrame() const
{
	return myData->myFrames[myCurrentFrame];
}

unsigned Animation::GetLastFrameIndex() const
{
	return myData->myLength - 1;
}

unsigned Animation::GetCurrentFrameIndex() const
{
	return myCurrentFrame;
}

bool Animation::IsValid() const
{
	return myData != nullptr;
}

const AnimationData& Animation::GetData() const
{
	return *myData;
}

void Animation::UpdateBoneCacheInternal(const Skeleton* aSkeleton, std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& outBones, unsigned anIndex) const
{
	const auto& bone = aSkeleton->GetBone(anIndex);
	outBones[anIndex] = bone.myBindPoseInverse * myData->myFrames[myCurrentFrame].myGlobalTransforms.at(bone.myNamespaceName);
	for (auto& childIndex : bone.myChildren)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, childIndex);
	}
}
