#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"

struct Bone
{
	Crimson::Matrix4x4f bindPoseInverse;
	std::vector<unsigned int> children;
	std::string namespaceName;
	std::string name;
	int parent;

	Bone() = default;
	Bone(const TGA::FBX::Skeleton::Bone& aBone);
};