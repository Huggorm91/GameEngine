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

	void SetSkeleton(Skeleton* aSkeleton);

	void SetCameraSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void Activate();

private:
	bool myIsActive;

	Crimson::Vector2f myWindowSize;

	Skeleton* mySkeleton;
	const Bone* mySelectedBone;
	const Bone* myHoveredBone;
	const Bone* myRootBone;

	Transform mySkeletonOffset;
	GameObject myCamera;

	Texture myAssetIcon;

	std::unordered_map<const Bone*, LineHandle> myLines;
	std::unordered_set<std::string> myAvailableFiles;

	void CreateMenubar();
	void CreateViewport();

	void CreateSkeletonHeirarchy();
	void CreateBoneList(const Bone& aBone);

	void CreateAssetBrowser();

	void UpdateAvailableFiles();

	void CreateBoneLines(unsigned anIndex, const Crimson::Vector4f& aParentPosition);
};