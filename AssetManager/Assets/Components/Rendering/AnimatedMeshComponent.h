#pragma once
#include "MeshComponent.h"
#include "../../Animation.h"	
#include "../ComponentParts/Skeleton.h"

class AnimatedMeshComponent: public MeshComponent
{
public:
	enum class AnimationState
	{
		Stopped,
		Looping,
		PlayOnce
	};

	AnimatedMeshComponent();
	AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, const std::string* aPath, Skeleton* aSkeleton);
	AnimatedMeshComponent(const AnimatedMeshComponent& aMeshComponent);
	~AnimatedMeshComponent() = default;

	void Update() override;

	void Init(const Json::Value& aJson) override;
	void Init(std::vector<MeshElement>& anElementList, const std::string& aName, const std::string* aPath, Skeleton* aSkeleton);

	void SetAnimation(const Animation& anAnimation);

	void StartAnimation(bool aIsLooping = false);
	void StopAnimation();
	void PauseAnimation();

	bool HasSkeleton() const;
	void SetSkeleton(Skeleton* aSkeleton);
	const Skeleton& GetSkeleton() const;
	const std::array<CommonUtilities::Matrix4x4f, 128>& GetBoneTransforms() const;

	Json::Value ToJson() const override;
	const AnimatedMeshComponent* GetTypePointer() const override;

private:
	std::array<CommonUtilities::Matrix4x4f, 128> myBoneTransformCache;
	Skeleton* mySkeleton;

	Animation myAnimation;
	float myAnimationTimer;
	unsigned int myCurrentFrame;
	AnimationState myAnimationState;

	void UpdateCache();
	void UpdateHeirarchy(unsigned int anIndex, const CommonUtilities::Matrix4x4f& aParentMatrix);
	const CommonUtilities::Matrix4x4f& GetLocalTransform(const Bone& aBone, const AnimationFrame& aFrame) const;
};

