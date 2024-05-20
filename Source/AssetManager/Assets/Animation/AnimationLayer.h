#pragma once
#include "Animation.h"

class AnimationLayer: public Animation
{
public:
	AnimationLayer();
	AnimationLayer(const Animation& anAnimation, unsigned aBoneIndex = 0);
	AnimationLayer(const AnimationLayer& anAnimation) = default;
	~AnimationLayer() = default;

	// AnimationLayer never alters transform, use Animation instead
	bool UpdateRootMotion(float aTimeSinceLastUpdate) override;
	// Will always return no motion, use Animation instead
	QuaternionTransform GetRootMotion(float aPercentage) override;

	using Animation::Init;
	void Init(const Json::Value& aJson) override;

	unsigned GetStartBoneIndex() const override;

	void SetBoneIndex(unsigned anIndex);

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const override;

	std::unordered_map<std::string, QuaternionTransform> GetFrameTransforms() const override;
	std::unordered_map<std::string, QuaternionTransform> GetFrameTransforms(float anInterpolationValue) const override;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

	Json::Value ToJson() const override;

private:
	unsigned myBoneIndex;

	void GetFrameTransformsInternal(std::unordered_map<std::string, QuaternionTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const;
	void GetFrameTransformsInternal(std::unordered_map<std::string, QuaternionTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame,float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const;
};

