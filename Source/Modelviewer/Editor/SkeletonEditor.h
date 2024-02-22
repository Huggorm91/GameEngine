#pragma once
#include "AssetManager/Assets/GameObject.h"
#include "AssetManager/Assets/Animation/Skeleton.h"
#include "GraphicsEngine/Drawer/LineHandle.h"

class SkeletonEditor
{
public:
	SkeletonEditor();
	~SkeletonEditor() = default;

	void Init(float aFoVDegree, float aNearPlane, float aFarPlane, float aCameraSpeed, float aMouseSensitivity);
	void Update();

	void SetSkeleton(Skeleton* aSkeleton, bool aHideLines = true);
	void SetAnimation(Animation anAnimation);

	void SetCameraSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void Activate();

private:
	bool myIsActive;

	Crimson::Vector2f myWindowSize;

	Animation myAnimation;
	Skeleton* mySkeleton;
	const Bone* mySelectedBone;
	const Bone* myHoveredBone;
	const Bone* myRootBone;

	Transform mySkeletonOffset;
	GameObject myCamera;

	Texture mySkeletonIcon;
	Texture myAnimationIcon;

	std::unordered_map<const Bone*, LineHandle> myLines;
	std::unordered_set<std::string> myAvailableSkeletons;
	std::unordered_set<std::string> myAvailableAnimations;

	void CreateMenubar();
	void CreateViewport();

	void CreateSkeletonHeirarchy();
	void CreateBoneList(const Bone& aBone);

	void CreateAssetBrowser();
	bool CreateFileButton(const std::string& aFile, float anIconSize, bool anIsAnimation);

	void UpdateAvailableFiles();

	void CreateBoneLines(unsigned anIndex, const Crimson::Vector4f& aParentPosition);
};