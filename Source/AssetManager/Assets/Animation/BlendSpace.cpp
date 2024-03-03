#include "AssetManager.pch.h"
#include "BlendSpace.h"

BlendSpace::BlendSpace(): AnimationBase(AnimationType::BlendSpace), myBlendValueGetter(nullptr), myBlendValue(0.f)
{}

bool BlendSpace::Update()
{
	return false;
}

const std::string& BlendSpace::GetName() const
{
	return myName;
}

void BlendSpace::SetToFirstFrame()
{}

void BlendSpace::SetToLastFrame()
{}

bool BlendSpace::NextFrame()
{
	return false;
}

bool BlendSpace::PreviousFrame()
{
	return false;
}

void BlendSpace::UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const
{
	aSkeleton; outBones;
}

void BlendSpace::UpdateBoneCache(const Skeleton * aSkeleton, BoneCache & outBones, float anInterpolationValue) const
{
	aSkeleton; outBones; anInterpolationValue;
;}

bool BlendSpace::IsValid() const
{
	return false;
}

bool BlendSpace::HasData() const
{
	return false;
}

bool BlendSpace::IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage) const
{
	aSkeleton; outErrorMessage;
	return false;
}

std::shared_ptr<AnimationBase> BlendSpace::GetAsSharedPtr() const
{
	return std::make_shared<BlendSpace>(*this);
}
