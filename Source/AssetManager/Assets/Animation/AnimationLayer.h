#pragma once
#include "Animation.h"

class AnimationLayer: public Animation
{
public:
	AnimationLayer();
	AnimationLayer(const Animation& anAnimation, unsigned aBoneIndex = 0);
	AnimationLayer(const AnimationLayer& anAnimation) = default;
	~AnimationLayer() = default;

	using Animation::Init;
	void Init(const Json::Value& aJson) override;

	unsigned GetStartBoneIndex() const override;

	void SetBoneIndex(unsigned anIndex);

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const override;

	std::unordered_map<std::string, AnimationTransform> GetFrameTransforms() const override;
	std::unordered_map<std::string, AnimationTransform> GetFrameTransforms(float anInterpolationValue) const override;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

	Json::Value ToJson() const override;

private:
	unsigned myBoneIndex;

	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const;
	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const;

	void GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const;
	void GetFrameTransformsInternal(std::unordered_map<std::string, AnimationTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame,float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const;
};

