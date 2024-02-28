#include "AssetManager.pch.h"
#include "Animation.h"
#include "Skeleton.h"

AnimationFrame::AnimationFrame(const TGA::FBX::Animation::Frame& aFrame) : globalTransformMatrices(), localTransformMatrices(), socketTransforms(), triggeredEvents(aFrame.TriggeredEvents)
{
	for (auto& [key, value] : aFrame.GlobalTransforms)
	{
		globalTransformMatrices.emplace(key, ConvertMatrix(value));
	}

	for (auto& [key, value] : aFrame.LocalTransforms)
	{
		localTransformMatrices.emplace(key, ConvertMatrix(value));
	}

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
	return myData->name;
}

float Animation::GetFPS() const
{
	return myData->framesPerSecond;
}

float Animation::GetFrameDelta() const
{
	return myData->frameDelta;
}

unsigned Animation::GetFrameCount() const
{
	return myData->length;
}

void Animation::SetToFirstFrame()
{
	myCurrentFrame = 1;
}

void Animation::SetToLastFrame()
{
	myCurrentFrame = myData->length - 1;
}

void Animation::SetFrameIndex(unsigned anIndex)
{
	myCurrentFrame = Crimson::Min(anIndex, GetLastFrameIndex());
}

bool Animation::NextFrame()
{
	if (++myCurrentFrame == myData->length)
	{
		SetToFirstFrame();
	}
	if (myCurrentFrame == myData->length -1)
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
	return myData->frames[anIndex];
}

const AnimationFrame& Animation::GetCurrentFrame() const
{
	return myData->frames[myCurrentFrame];
}

unsigned Animation::GetLastFrameIndex() const
{
	return myData->length - 1;
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
	outBones[anIndex] = bone.bindPoseInverse * myData->frames[myCurrentFrame].globalTransformMatrices.at(bone.namespaceName);
	for (auto& childIndex : bone.children)
	{
		UpdateBoneCacheInternal(aSkeleton, outBones, childIndex);
	}
}
