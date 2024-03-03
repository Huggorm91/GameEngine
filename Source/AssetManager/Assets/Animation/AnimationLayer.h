#pragma once
#include "Animation.h"

class AnimationLayer: public Animation
{
public:
	AnimationLayer();
	AnimationLayer(AnimationData& someData, unsigned aBoneIndex = 0);
	AnimationLayer(const Animation& anAnimation, unsigned aBoneIndex = 0);
	AnimationLayer(const AnimationLayer& anAnimation) = default;
	~AnimationLayer() = default;

	void SetBoneIndex(unsigned anIndex);
	unsigned GetBoneIndex() const;

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const override;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

private:
	unsigned myBoneIndex;
};

