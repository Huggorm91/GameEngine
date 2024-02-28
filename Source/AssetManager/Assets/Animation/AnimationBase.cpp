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
	return AnimationTransform{ Lerp(aFrom.position, aTo.position, aPercentage), Slerp(aFrom.rotation, aTo.rotation, aPercentage) };
}

AnimationFrame::AnimationFrame(const TGA::FBX::Animation::Frame& aFrame) : globalTransformMatrices(), localTransformMatrices(), socketTransforms(), triggeredEvents(aFrame.TriggeredEvents)
{
	for (auto& [key, value] : aFrame.GlobalTransforms)
	{
		const auto& matrix = ConvertMatrix(value);
		globalTransformMatrices.emplace(key, matrix);

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
