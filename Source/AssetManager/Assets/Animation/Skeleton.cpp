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
	return myIndexMap.find(aName) != myIndexMap.end();
}

const Bone& Skeleton::GetBone(unsigned int anIndex) const
{
	return myBones[anIndex];
}

const Bone& Skeleton::GetBone(const std::string& aName) const
{
	return myBones[myIndexMap.at(aName)];
}

const std::vector<Bone>& Skeleton::GetBones() const
{
	return myBones;
}

unsigned Skeleton::GetBoneCount() const
{
	return static_cast<unsigned>(myBones.size());
}

unsigned Skeleton::GetSocketCount() const
{
	return static_cast<unsigned>(mySockets.size());
}

bool Skeleton::HasSocket(const std::string& aName) const
{
	return mySockets.find(aName) != mySockets.end();
}

const Socket& Skeleton::GetSocket(const std::string& aName) const
{
	return mySockets.at(aName);
}