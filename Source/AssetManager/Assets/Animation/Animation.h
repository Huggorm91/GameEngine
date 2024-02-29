#pragma once
#include "AnimationBase.h"

class Animation: public AnimationBase
{
public:
	Animation();
	Animation(AnimationData& someData);
	~Animation() = default;

	bool operator==(const Animation& anAnimation) const;

	const std::string& GetName() const override;

	float GetFPS() const override;
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
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue, bool anInterpolatePreviousFrame = false) const override;

	const AnimationFrame& GetFrame(unsigned int anIndex) const;
	const AnimationFrame& GetCurrentFrame() const;
	unsigned GetLastFrameIndex() const;
	unsigned GetCurrentFrameIndex() const;

	bool IsValid() const override;
	const AnimationData& GetData() const;

	//void Serialize(std::ostream& aStream) const override;
	//void Deserialize(std::istream& aStream) override;

protected:
	AnimationData* myData;
	unsigned myCurrentFrame;

	const AnimationFrame& GetNextFrame() const;
	const AnimationFrame& GetPreviousFrame() const;

private:
	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aFrame) const;
	void UpdateBoneCacheInternal(const Skeleton* aSkeleton, BoneCache& outBones, unsigned anIndex, const AnimationFrame& aCurrentFrame, const AnimationFrame& aInterpolationFrame, float anInterpolationValue) const;
};