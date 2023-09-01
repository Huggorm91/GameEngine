#include "AssetManager.pch.h"
#include "Skeleton.h"

Skeleton::Skeleton(const TGA::FBX::Skeleton& aSkeleton): myName(aSkeleton.Name), myPath(nullptr), myIndexMap(aSkeleton.BoneNameToIndex), myBones(), mySockets()
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

const std::string& Skeleton::GetName() const
{
	return myName;
}

const std::string& Skeleton::GetPath() const
{
	return *myPath;
}

void Skeleton::SetPath(const std::string* aPath)
{
	myPath = aPath;
}

bool Skeleton::HasBone(const std::string& aName) const
{
	auto iter = myIndexMap.find(aName);
	return iter != myIndexMap.end();
}

const Bone& Skeleton::GetBone(unsigned int anIndex) const
{
	return myBones[anIndex];
}

const Bone& Skeleton::GetBone(const std::string& aName) const
{
	auto iter = myIndexMap.find(aName);
	assert(iter != myIndexMap.end() && "Bone does not exist in Skeleton!");
	return myBones[iter->second];
}

const std::vector<Bone>& Skeleton::GetBones() const
{
	return myBones;
}
