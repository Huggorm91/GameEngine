#pragma once
#include "TgaImporterConversions.h"

struct Bone
{
	CommonUtilities::Matrix4x4f myBindPoseInverse;
	std::vector<unsigned int> myChildren;
	std::string myNamespaceName;
	std::string myName;
	int myParent;

	Bone(const TGA::FBX::Skeleton::Bone& aBone);
};