#include "AssetManager.pch.h"
#include "Skeleton.h"

Skeleton::Skeleton(const TGA::FBX::Skeleton& aSkeleton): myName(aSkeleton.Name), myIndexMap(aSkeleton.BoneNameToIndex), myBones(), mySockets()
{
	for (auto& bone : aSkeleton.Bones)
	{
		myBones.emplace_back(bone);
	}

	for (auto& [key, value] : aSkeleton.Sockets)
	{
		mySockets.emplace(key, value);
	}
}