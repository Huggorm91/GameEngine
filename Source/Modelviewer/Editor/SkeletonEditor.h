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
	void SetActive(bool aState);

	void SetCameraSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

private:
	bool myIsActive;

	Skeleton* mySkeleton;
	GameObject myCamera;

	std::vector<LineHandle> myLines;
};