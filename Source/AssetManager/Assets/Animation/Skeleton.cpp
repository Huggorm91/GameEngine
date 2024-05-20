#include "AssetManager.pch.h"
#include "Skeleton.h"

Skeleton::Skeleton(const TGA::FBX::Skeleton& aSkeleton): myName(aSkeleton.Name), myPath(nullptr), myIndexMap(aSkeleton.BoneNameToIndex), myBones(), mySockets()
{
	for (auto& bone : aSkeleton.Bones)
	{
		auto& newBone = myBones.emplace_back(bone);
		if (newBone.parent >= 0)
		{
			newBone.localBindPoseInverse = myBones[newBone.parent].bindPoseInverse.GetInverse() * newBone.bindPoseInverse;
		}
	}

	for (auto& [key, value] : aSkeleton.Sockets)
	{
		mySockets.emplace(key, value);
	}
}

Skeleton::Skeleton(std::istream& aStream) : myPath(nullptr)
{
	std::getline(aStream, myName, '\0');

	// Bones
	unsigned boneCount;
	aStream.read(reinterpret_cast<char*>(&boneCount), sizeof(boneCount));
	myBones.resize(boneCount);
	aStream.read(reinterpret_cast<char*>(myBones.data()), boneCount * sizeof(Bone));

	// Index map
	unsigned indexCount;
	aStream.read(reinterpret_cast<char*>(&indexCount), sizeof(indexCount));
	for (unsigned i = 0; i < indexCount; i++)
	{
		std::string name;
		std::getline(aStream, name, '\0');
		size_t index;
		aStream.read(reinterpret_cast<char*>(&index), sizeof(index));

		myIndexMap.emplace(name, index);
	}

	// Sockets
	unsigned socketCount;
	aStream.read(reinterpret_cast<char*>(&socketCount), sizeof(socketCount));
	for (unsigned i = 0; i < socketCount; i++)
	{
		std::string name;
		std::getline(aStream, name, '\0');
		mySockets.emplace(name, aStream);
	}
}

void Skeleton::Serialize(std::ostream& aStream) const
{
	aStream.write(myName.c_str(), myName.size() + 1);

	// Bones
	unsigned boneCount = static_cast<unsigned>(myBones.size());
	aStream.write(reinterpret_cast<const char*>(&boneCount), sizeof(boneCount));
	aStream.write(reinterpret_cast<const char*>(myBones.data()), boneCount * sizeof(Bone));

	// Index map
	unsigned indexCount = static_cast<unsigned>(myIndexMap.size());
	aStream.write(reinterpret_cast<const char*>(&indexCount), sizeof(indexCount));
	for (auto& [name, index] : myIndexMap)
	{
		aStream.write(name.c_str(), name.size() + 1);
		aStream.write(reinterpret_cast<const char*>(&index), sizeof(index));
	}

	// Sockets
	unsigned socketCount = static_cast<unsigned>(mySockets.size());
	aStream.write(reinterpret_cast<const char*>(&socketCount), sizeof(socketCount));
	for (auto& [name, socket] : mySockets)
	{
		aStream.write(name.c_str(), name.size() + 1);
		socket.Serialize(aStream);
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

const Bone& Skeleton::GetBone(unsigned anIndex) const
{
	return myBones[anIndex];
}

const Bone& Skeleton::GetBone(const std::string& aName) const
{
	return myBones[myIndexMap.at(aName)];
}

unsigned Skeleton::GetBoneIndex(const std::string& aName) const
{
	return static_cast<unsigned>(myIndexMap.at(aName));
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