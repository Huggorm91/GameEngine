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

	void AddAnimation(const Animation& anAnimation, float aBlendValue);
	void AddAnimation(const AnimationLayer& anAnimation, float aBlendValue);
	void AddAnimation(const Animation& anAnimation, unsigned aBoneIndex, float aBlendValue); // Will add the Animation as an AnimationLayer

	void RemoveAnimation(const Animation& anAnimation, float aBlendValue);
	void RemoveAnimation(const AnimationLayer& anAnimation, float aBlendValue);

	void SetToFirstFrame() override;
	void SetToLastFrame() override;

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	bool NextFrame() override;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	bool PreviousFrame() override;

	void SetBlendValue(float aValue);

	// Will override any manually assigned blendvalues in Update
	void SetBlendValueGetter(const std::function<float()>& aFunction);

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue) const override;

	bool IsValid() const override;
	bool HasData() const override;

	bool IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage = nullptr) const override;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

private:
	struct BlendData
	{
		Animation* animation = nullptr;
		float blendValue = 0.f;

		~BlendData()
		{
			delete animation;
		}
		bool operator<(const BlendData& someData) const
		{
			return blendValue < someData.blendValue;
		}
	};
	std::vector<BlendData> myAnimations;
	std::string myName;

	std::function<float()> myBlendValueGetter;
	float myBlendValue;
};

