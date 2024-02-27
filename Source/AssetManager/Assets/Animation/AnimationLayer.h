#pragma once
#include "Animation.h"

class AnimationLayer: public Animation
{
public:
	AnimationLayer();
	AnimationLayer(AnimationData& someData, unsigned aBoneIndex);
	~AnimationLayer() = default;

	void UpdateBoneCache(const Skeleton* aSkeleton, std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& outBones) const override;

private:
	unsigned myBoneIndex;

	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& outBones, unsigned anIndex) const;
};

