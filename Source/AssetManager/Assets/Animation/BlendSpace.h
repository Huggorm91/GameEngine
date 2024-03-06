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

	bool IsValidSkeleton(const Skeleton* aSkeleton, std::string* outErrorMessage = nullptr) const override;

	std::shared_ptr<AnimationBase> GetAsSharedPtr() const override;

private:
	struct BlendData
	{
		std::shared_ptr<Animation> animation;
		float blendValue;
		const unsigned id;

		BlendData(std::shared_ptr<Animation> anAnimation, float aValue, unsigned anID) : animation(anAnimation), blendValue(aValue), id(anID) {}
		inline BlendData& operator=(const BlendData& someData)
		{
			animation = someData.animation;
			blendValue = someData.blendValue;
			const_cast<unsigned&>(id) = someData.id;
			return *this;
		}
		inline bool operator<(const BlendData& someData) const
		{
			return blendValue < someData.blendValue;
		}
	};

	std::vector<BlendData> myAnimations;
	std::unordered_map<unsigned, float> myTimers;

	std::string myName;

	const Animation* myLongestAnimation;

	std::function<float()> myBlendValueGetter;
	float myBlendValue;

	unsigned myBlendDataCounter;
	bool myHasMatchingFPS;

	void AddInternal();
	void UpdateMatchingFPS();
	void CreateTimers();

	void UpdateAnimations();
};

