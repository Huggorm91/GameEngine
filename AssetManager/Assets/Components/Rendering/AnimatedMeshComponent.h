#pragma once
#include "MeshComponent.h"
#include "../../Animation.h"	

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
	AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, int aSkeletonIndex);
	AnimatedMeshComponent(const AnimatedMeshComponent& aMeshComponent);
	~AnimatedMeshComponent() = default;

	void Update() override;

	// Call after binding it to a GameObject
	void Init();
	void Init(std::vector<MeshElement>& anElementList, const std::string& aName, int aSkeletonIndex);

	void SetAnimation(Animation& anAnimation);
	void SetAnimation(Animation* anAnimation);

	void StartAnimation(bool aIsLooping = false);
	void StopAnimation();
	void PauseAnimation();

	bool HasSkeleton() const;
	void BindSkelton(int aSkeletonIndex);
	const SkeletonComponent& GetSkeleton() const;
	const std::array<CommonUtilities::Matrix4x4f, 128>& GetBoneTransforms() const;

	void ComponentPointersInvalidated() override;
	const AnimatedMeshComponent* GetTypePointer() const override;

private:
	std::array<CommonUtilities::Matrix4x4f, 128> myBoneTransformCache;
	SkeletonComponent* mySkeleton;

	Animation* myAnimation;
	float myAnimationTimer;
	unsigned int myCurrentFrame;
	AnimationState myAnimationState;

	int mySkeletonIndex;

	void UpdateCache();
	void UpdateHeirarchy(unsigned int anIndex, const CommonUtilities::Matrix4x4f& aParentMatrix);
	const CommonUtilities::Matrix4x4f& GetLocalTransform(const Bone& aBone, const AnimationFrame& aFrame) const;
};

