#pragma once
#include "AnimationBase.h"

class AnimationLayer;

class Animation: public AnimationBase
{
public:
	Animation();
	Animation(const AnimationData* someData);
	Animation(const Animation& anAnimation);
	Animation(const AnimationLayer& anAnimation);
	Animation(Animation&& anAnimation) noexcept;
	~Animation() = default;

	bool operator==(const Animation& anAnimation) const;

	// Returns true while playing
	bool Update() override;

	/// <param name="aTimeSinceLastUpdate">The time since last this function was called, if called every frame it is the deltatime</param>
	/// <returns>True if the animation altered the transform</returns>
	bool UpdateRootMotion(float aTimeSinceLastUpdate)override;

	/// <param name="aPercentage">The amount of the total motion from the current frame to get, from 0.f to 1.f</param>
	/// <returns>The interpolated motion</returns>
	QuaternionTransform GetRootMotion(float aPercentage) override;

	using AnimationBase::Init;
	void Init(const Json::Value& aJson) override;

	const std::string& GetPath() const override;
	unsigned GetStartBoneIndex() const override;

	float GetFPS() const;
	float GetFrameDelta() const override;
	unsigned GetFrameCount() const;

	void SetToFirstFrame() override;
	void SetToLastFrame() override;
	void SetFrameIndex(unsigned anIndex);

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	bool NextFrame() override;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	bool PreviousFrame() override;

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const override;

	const AnimationFrame& GetFrame(unsigned int anIndex) const;
	const AnimationFrame& GetCurrentFrame() const;
	const AnimationFrame& GetNextFrame() const;
	const AnimationFrame& GetPreviousFrame() const;

	unsigned GetLastFrameIndex() const;
	unsigned GetCurrentFrameIndex() const;

	bool IsEndOfLoop() const override;

	bool IsValid() const override;
	bool HasData() const override;

	bool IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage = nullptr) const override;

	const AnimationData& GetData() const;

	std::unordered_map<std::string, QuaternionTransform> GetAdditiveTransforms() const override;
	std::unordered_map<std::string, QuaternionTransform> GetFrameTransforms() const override;
	std::unordered_map<std::string, QuaternionTransform> GetFrameTransforms(float anInterpolationValue) const override;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

	//void Serialize(std::ostream& aStream) const override;
	//void Deserialize(std::istream& aStream) override;

	void ValidateUsingNamespace(const Skeleton* aSkeleton) override;

protected:
	const AnimationData* myData;
	unsigned myCurrentFrame;

	Animation(AnimationType aType);

	void UpdateBoneCacheGlobal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame) const;
	void UpdateBoneCacheGlobal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue) const;

	void UpdateBoneCacheLocal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const;
	void UpdateBoneCacheLocal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const;

	void GetAdditiveTransformsInternal(std::unordered_map<std::string, QuaternionTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aFrame, const Crimson::Matrix4x4f& aParentTransform) const;
	void GetAdditiveTransformsInternal(std::unordered_map<std::string, QuaternionTransform>& outTransforms, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue, const Crimson::Matrix4x4f& aParentTransform) const;
};