#pragma once
#include "AnimationLayer.h"
#include <functional>

class BlendState: public AnimationBase
{
public:
	~BlendState() = default;

	const std::string& GetName() const override;

	float GetFPS() const override;
	float GetFrameDelta() const override;

	void SetToFirstFrame() override;
	void SetToLastFrame() override;

	// Returns false if the new frame is the last, will loop to the first frame if called after returning false
	bool NextFrame() override;
	// Returns false if the new frame is the first, will loop to the last frame if called after returning false
	bool PreviousFrame() override;

	void SetBlendValue(float aValue);
	void SetBlendValueGetter(const std::function<float()>& aFunction);

	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones) const override;
	void UpdateBoneCache(const Skeleton* aSkeleton, BoneCache& outBones, float anInterpolationValue, bool anInterpolatePreviousFrame = false) const override;

	bool IsValid() const override;

private:
	struct BlendData
	{
		Animation* animation;
		float blendValue;

		bool operator<(const BlendData& someData) const
		{
			return blendValue < someData.blendValue;
		}
	};
	std::vector<BlendData> myAnimations;
	std::function<float()> myBlendValueGetter; // https://en.cppreference.com/w/cpp/utility/functional/function/operator_bool
	float myBlendValue;
};

