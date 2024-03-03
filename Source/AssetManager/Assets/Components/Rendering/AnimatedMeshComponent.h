#pragma once
#include "MeshComponent.h"
#include "..\..\Animation\Animation.h"	
#include "..\..\Animation\Skeleton.h"

class AnimatedMeshComponent: public MeshComponent
{
public:
	enum class AnimationState
	{
		PlayOnce,
		Looping,
		Stopped,
	};

	AnimatedMeshComponent();
	AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, Skeleton* aSkeleton);
	AnimatedMeshComponent(const AnimatedMeshComponent& aMeshComponent);
	AnimatedMeshComponent(AnimatedMeshComponent&& aMeshComponent) = default;
	~AnimatedMeshComponent() = default;
	AnimatedMeshComponent& operator=(const AnimatedMeshComponent& aComponent);
	AnimatedMeshComponent& operator=(AnimatedMeshComponent&& aComponent) noexcept = default;

	void Update() override;
	void Render() override;

	void Init(GameObject* aParent) override;
	void Init(const Json::Value& aJson) override;
	void Init(std::vector<MeshElement>& anElementList, const std::string& aName, Skeleton* aSkeleton);

	void SetLooping(bool aIsLooping);
	void ToogleLooping();
	bool IsLooping() const;

	void SetPlayInReverse(bool aShouldPlayInReverse);

	void SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation);
	void SetTargetFPS(float aFPS);

	void StartAnimation();
	void StopAnimation();
	void PauseAnimation();

	void ResetBoneCache();

	bool HasSkeleton() const;
	void SetSkeleton(Skeleton* aSkeleton);
	const Skeleton& GetSkeleton() const;
	const std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& GetBoneTransforms() const;

	void CreateImGuiComponents(const std::string& aWindowName) override;

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const AnimatedMeshComponent* GetTypePointer() const override;

private:
	std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> myBoneTransformCache;

	Skeleton* mySkeleton;

	std::shared_ptr<AnimationBase> myAnimation;
	AnimationState myAnimationState;
};

