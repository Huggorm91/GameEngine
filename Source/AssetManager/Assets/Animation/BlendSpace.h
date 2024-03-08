#pragma once
#include "AnimationLayer.h"
#include <functional>

class BlendSpace : public AnimationBase
{
public:
	BlendSpace();
	~BlendSpace() = default;

	bool Update() override;

	const std::string& GetName() const override;
	unsigned GetStartBoneIndex() const override;

	bool AddAnimation(const Animation& anAnimation, float aBlendValue);
	bool AddAnimation(const AnimationLayer& anAnimation, float aBlendValue);
	bool AddAnimation(const Animation& anAnimation, unsigned aBoneIndex, float aBlendValue); // Will add the Animation as an AnimationLayer

	bool RemoveAnimation(const Animation& anAnimation, float aBlendValue);

	void SetToFirstFrame() override;
	void SetToLastFrame() override;

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	bool NextFrame() override;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	bool PreviousFrame() override;

	// Will override any manually assigned blendvalues in Update
	void SetBlendValueGetter(const std::function<float()>& aFunction);
	void SetBlendValue(float aValue);

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const override;

	bool IsValid() const override;
	bool HasData() const override;

	void SetIsPlayingInReverse(bool aShouldPlayBackwards) override;

	bool IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage = nullptr) const override;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

private:
	struct BlendData
	{
		std::shared_ptr<Animation> animation;
		float blendValue;
		float timer;

		inline bool operator<(const BlendData& someData) const
		{
			return blendValue < someData.blendValue;
		}
	};

	std::vector<BlendData> myAnimations;

	std::string myName;

	const Animation* myLongestAnimation;

	std::function<float()> myBlendValueGetter;
	float myBlendValue;

	unsigned myBoneIndex;
	bool myHasMatchingFPS;

	void AddInternal();
	void UpdateMatchingFPS();

	void UpdateAnimations();
};

