#pragma once
#include "Bone.h"
#include "Socket.h"

class Skeleton
{
public:
	Skeleton() = default;
	Skeleton(const TGA::FBX::Skeleton& aSkeleton);
	~Skeleton() = default;

	const std::string& GetName() const;
	const std::string& GetPath() const;
	void SetPath(const std::string* aPath);

	bool HasBone(const std::string& aName) const;
	const Bone& GetBone(unsigned anIndex) const;
	const Bone& GetBone(const std::string& aName) const;

	unsigned GetBoneIndex(const std::string& aName) const;

	const std::vector<Bone>& GetBones() const;

	unsigned GetBoneCount() const;
	unsigned GetSocketCount() const;

	bool HasSocket(const std::string& aName) const;
	const Socket& GetSocket(const std::string& aName) const;

private:
	const std::string* path;
	std::string myName;
	std::vector<Bone> myBones;
	std::unordered_map<std::string, size_t> myIndexMap;
	std::unordered_map<std::string, Socket> mySockets;
};

