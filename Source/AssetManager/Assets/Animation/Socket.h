#pragma once
#include "../Components/ComponentParts/TgaImporterConversions.h"

struct Socket
{
	Crimson::Matrix4x4f myRestTransform;
	std::string myNamespaceName;
	std::string name;
	int myParentBoneIdx;

	Socket(TGA::FBX::Skeleton::Socket aSocket);
};