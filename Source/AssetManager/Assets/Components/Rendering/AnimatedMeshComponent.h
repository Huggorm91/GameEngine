#pragma once
#include "MeshComponent.h"
#include "..\..\Animation\Animation.h"	
#include "..\..\Animation\Skeleton.h"

BEGIN_COMPONENT(AnimatedMeshComponent, MeshComponent)
public:
	AnimatedMeshComponent();
	AnimatedMeshComponent(ComponentType aType);
	AnimatedMeshComponent(const Json::Value& aJson);
	AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, Skeleton* aSkeleton);
	AnimatedMeshComponent(const AnimatedMeshComponent& aMeshComponent);
	AnimatedMeshComponent(AnimatedMeshComponent&& aMeshComponent) = default;
	virtual ~AnimatedMeshComponent() = default;
	AnimatedMeshComponent& operator=(const AnimatedMeshComponent& aComponent);
	AnimatedMeshComponent& operator=(AnimatedMeshComponent&& aComponent) noexcept = default;

	void Update() override;
	void Render() override;

	void Init(GameObject* aParent) override;
	void Init(std::vector<MeshElement>& anElementList, const std::string& aName, Skeleton* aSkeleton);

	virtual void SetLooping(bool aIsLooping);
	virtual void ToogleLooping();
	bool IsLooping() const;

	virtual void SetPlayInReverse(bool aShouldPlayInReverse);

	virtual void SetTargetFPS(float aFPS);

	virtual void SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation);

	virtual void StartAnimation();
	virtual void StopAnimation();
	virtual void PauseAnimation();

	void ResetBoneCache();

	bool HasSkeleton() const;
	void SetSkeleton(Skeleton* aSkeleton);
	const Skeleton& GetSkeleton() const;
	const std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& GetBoneTransforms() const;

	void CreateImGuiComponents(const std::string& aWindowName) override;

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

	Json::Value ToJson() const override;

protected:
	std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> myBoneTransformCache;
	std::shared_ptr<AnimationBase> myAnimation;
	Skeleton* mySkeleton;
};