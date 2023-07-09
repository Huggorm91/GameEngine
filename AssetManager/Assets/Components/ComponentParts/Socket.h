#pragma once
#include "TgaImporterConversions.h"

struct Socket
{
	CommonUtilities::Matrix4x4f myRestTransform;
	std::string myNamespaceName;
	std::string myName;
	int myParentBoneIdx;

	Socket(TGA::FBX::Skeleton::Socket aSocket);
};