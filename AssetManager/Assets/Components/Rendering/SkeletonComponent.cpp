#include "AssetManager.pch.h"
#include "SkeletonComponent.h"

SkeletonComponent::SkeletonComponent(): mySkeleton(nullptr)
{
}

SkeletonComponent::SkeletonComponent(Skeleton& aSkeleton): mySkeleton(&aSkeleton)
{
}

const std::string& SkeletonComponent::GetName() const
{
	return mySkeleton->myName;
}

bool SkeletonComponent::HasBone(const std::string& aName) const
{
	auto iter = mySkeleton->myIndexMap.find(aName);
	return iter != mySkeleton->myIndexMap.end();
}

const Bone& SkeletonComponent::GetBone(unsigned int anIndex) const
{
	return mySkeleton->myBones[anIndex];
}

const Bone& SkeletonComponent::GetBone(const std::string& aName) const
{
	auto iter = mySkeleton->myIndexMap.find(aName);
	assert(iter != mySkeleton->myIndexMap.end() && "Bone does not exist in Skeleton!");
	return mySkeleton->myBones[iter->second];
}

const std::vector<Bone>& SkeletonComponent::GetBones() const
{
	return mySkeleton->myBones;
}

const SkeletonComponent* SkeletonComponent::GetTypePointer() const
{
	return this;
}
