#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"
#include "CrimsonUtilities/Math/QuaternionTransform.h"

struct Bone
{
	Crimson::Matrix4x4f bindPoseInverse;
	Crimson::Matrix4x4f localBindPoseInverse;
	QuaternionTransform bindPose;
	std::vector<unsigned int> children;
	std::string namespaceName;
	std::string name;
	int parent;

	Bone() = default;
	Bone(const TGA::FBX::Skeleton::Bone& aBone);
};