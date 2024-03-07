#pragma once
#include "Animation.h"

class AnimationLayer: public Animation
{
public:
	AnimationLayer();
	AnimationLayer(const Animation& anAnimation, unsigned aBoneIndex = 0);
	AnimationLayer(const AnimationLayer& anAnimation) = default;
	~AnimationLayer() = default;

	void SetBoneIndex(unsigned anIndex);
	unsigned GetBoneIndex() const;

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const override;

	virtual std::unordered_map<std::string, AnimationTransform> GetFrameTransforms();
	virtual std::unordered_map<std::string, AnimationTransform> GetFrameTransforms(float anInterpolationValue);

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

private:
	unsigned myBoneIndex;

	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const;
	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const;

	void GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform);
	void GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame,float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform);
};

