#pragma once
#include "AnimationBase.h"

class Animation: public AnimationBase
{
public:
	Animation();
	Animation(const Json::Value& aJson);
	Animation(AnimationData* someData);
	Animation(const Animation& anAnimation);
	Animation(Animation&& anAnimation) noexcept;
	~Animation() = default;

	bool operator==(const Animation& anAnimation) const;

	bool Update() override;

	const std::string& GetPath() const override;
	unsigned GetStartBoneIndex() const override;

	float GetFPS() const;
	float GetFrameDelta() const;
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

	bool IsValid() const override;
	bool HasData() const override;

	bool IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage = nullptr) const override;

	const AnimationData& GetData() const;
	virtual std::unordered_map<std::string, AnimationTransform> GetFrameTransforms() const;
	virtual std::unordered_map<std::string, AnimationTransform> GetFrameTransforms(float anInterpolationValue) const;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

	//void Serialize(std::ostream& aStream) const override;
	//void Deserialize(std::istream& aStream) override;

protected:
	AnimationData* myData;
	unsigned myCurrentFrame;

	Animation(AnimationType aType);

private:
	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame) const;
	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& anInterpolationFrame, float anInterpolationValue) const;
};