#pragma once
#include "AssetManager/Assets/GameObject.h"
#include "AssetManager/Assets/Animation/Skeleton.h"
#include "AssetManager/Assets/Animation/BlendSpace.h"
#include "GraphicsEngine/Drawer/LineHandle.h"

class SkeletonEditor
{
public:
	SkeletonEditor();
	~SkeletonEditor() = default;

	void Init(float aFoVDegree, float aNearPlane, float aFarPlane, float aCameraSpeed, float aMouseSensitivity);
	void Update();

	void SetSkeleton(Skeleton* aSkeleton, bool aHideLines = true);
	void SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation);

	void SetCameraSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void Activate();
	void Deactivate();

private:
	bool myIsActive;
	bool myHasMatchingBones;
	bool myIsPlayingAnimation;
	bool myIsPlayingInReverse;
	bool myShouldRenderMesh;
	bool myShouldRenderSkeleton;

	float myAnimationTimer;
	float myPlaybackMultiplier;
	float myTargetFPS;
	float myEditorTimeScale;

	Crimson::Vector2f myWindowSize;

	std::shared_ptr<AnimationBase> myAnimation;
	Skeleton* mySkeleton;
	Texture* myMeshTexture;
	AnimatedMeshComponent* myMesh; // Points to myModels AnimatedMeshComponent
	const Transform* mySkeletonOffset; // Points to myModels Transform

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

	std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> myBoneTransforms;

	void CreateMenubar();
	void CreateViewport();

	void CreateSkeletonInspector();
	void CreateBoneList(const Bone& aBone);
	void CreateTexturePicker();

	void CreateAnimationInspector();
	void CreateAnimationInfo(Animation* anAnimation);
	void CreateAnimationControls();
	void CreateAnimationParameters(Animation* anAnimation);
	void PrintAnimationMissmatchError();

	void CreateAssetBrowser();
	void CreateFileButton(const std::string& aFile, float anIconSize, bool anIsAnimation);

	void UpdateAvailableFiles();

	// Creates new handles in myLines
	void GenerateSkeletonDrawing();
	void GenerateSkeletonDrawing(unsigned anIndex, const Crimson::Vector4f& aParentPosition);

	void DrawSkeleton();
	void DrawSkeleton(unsigned anIndex, const Crimson::Vector4f& aParentPosition);

	void DrawFrame();
	void DrawFrame(unsigned anIndex, const Crimson::Vector4f& aParentPosition);

	void CheckSkeletonAnimationMatching();

	void ClearLines();

	unsigned GetBoneIndex(const Bone* aBone) const;
	void SetMeshAnimation();

	void SelectBone(const Bone* aBone);

	void LoadBlendSpace();
	void SaveBlendSpace();
};