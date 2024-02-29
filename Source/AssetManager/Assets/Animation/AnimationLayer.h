#pragma once
#include "Animation.h"

class AnimationLayer: public Animation
{
public:
	AnimationLayer();
	AnimationLayer(AnimationData& someData, unsigned aBoneIndex);
	~AnimationLayer() = default;

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue, bool anInterpolatePreviousFrame = false) const override;

private:
	unsigned myBoneIndex;

	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex) const;
};

