#pragma once
#include "AssetManager/Assets/GameObject.h"
#include "AssetManager/Assets/Animation/Skeleton.h"
#include "AssetManager/Assets/Animation/BlendSpace.h"
#include "GraphicsEngine/Drawer/LineHandle.h"
#include "AssetManager/Assets/Components/Rendering/AnimationControllerComponent.h"

class SkeletonEditor
{
public:
	SkeletonEditor();
	~SkeletonEditor() = default;

	void Init(float aFoVDegree, float aNearPlane, float aFarPlane, float aCameraSpeed, float aMouseSensitivity);
	void Update();

	void SetSkeleton(Skeleton* aSkeleton, bool aHideLines = true);
	void SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation);
	void AddAnimation(const std::shared_ptr<AnimationBase>& anAnimation);

	void SetCameraSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void Activate();
	void Deactivate();

private:
	bool myIsActive;
	bool myHasMatchingBones;
	bool myIsPlayingInReverse;
	bool myShouldRenderMesh;
	bool myShouldRenderSkeleton;
	bool myIsEditingBlendSpace;
	bool myIsUsingRootMotion;

	int myPlayCount;

	float myAnimationTimer;
	float myPlaybackMultiplier;
	float myTargetFPS;
	float myEditorTimeScale;

	Crimson::Vector2f myWindowSize;

	Skeleton* mySkeleton;
	Texture* myMeshTexture;
	AnimationControllerComponent* myMesh; // Points to myModel's AnimationControllerComponent
	std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>* myBoneTransforms; // Points to myMesh's BoneCache
	Transform* mySkeletonOffset; // Points to myModels Transform

	const Bone* mySelectedBone;
	const Bone* myHoveredBone;
	const Bone* myRootBone;

	Crimson::Vector4f myBoneColor;
	Crimson::Vector4f mySelectedColor;
	Crimson::Vector4f myHoveredColor;

	std::string myMissMatchMessage;
	std::string mySelectedAnimationPath;

	GameObject myCamera;
	GameObject myModel;

	Texture mySkeletonIcon;
	Texture myAnimationIcon;

	std::unordered_map<const Bone*, LineHandle> myLines;
	std::unordered_set<std::string> myAvailableSkeletons;
	std::unordered_set<std::string> myAvailableAnimations;

	void CreateMenubar();
	void CreateViewport();

	void CreateSkeletonInspector();
	void CreateBoneList(const Bone& aBone);
	void CreateTexturePicker();

	void CreateAnimationInspector();
	void CreateAnimationSection(Animation* anAnimation);
	void CreateAnimationLayerSection(AnimationLayer* anAnimation);
	void CreateBlendSpaceSection(BlendSpace* aBlendSpace);
	void CreateAnimationInfo(Animation* anAnimation);
	void CreateAnimationControls(AnimationBase* anAnimation);
	void CreateAnimationParameters(Animation* anAnimation);
	void PrintAnimationMissmatchError();

	void CreateAssetBrowser();
	void CreateFileButton(const std::string& aFile, float anIconSize, bool anIsAnimation);

	void SetAnimation(const std::string& aPath);
	void AddAnimation(const std::string& aPath);

	void UpdateAvailableFiles();

	// Creates new handles in myLines
	void GenerateSkeletonDrawing();
	void GenerateSkeletonDrawing(unsigned anIndex, const Crimson::Vector4f& aParentPosition);

	void DrawSkeleton();
	void DrawSkeleton(unsigned anIndex, const Crimson::Vector4f& aParentPosition);

	void DrawFrame();
	void DrawFrame(unsigned anIndex, const Crimson::Vector4f& aParentPosition);

	void CheckSkeletonAnimationMatching(const std::shared_ptr<AnimationBase>& anAnimation);

	void ClearLines();

	unsigned GetBoneIndex(const Bone* aBone) const;

	void SelectBone(const Bone* aBone);
	void SetBone(unsigned anIndex, std::shared_ptr<AnimationBase>& outAnimation);

	void LoadBlendSpace();
	void SaveBlendSpace();
};