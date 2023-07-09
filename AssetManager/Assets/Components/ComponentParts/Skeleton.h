#pragma once
#include "Bone.h"
#include "Socket.h"

struct Skeleton
{
	std::string myName;
	std::vector<Bone> myBones;
	std::unordered_map<std::string, size_t> myIndexMap;
	std::unordered_map<std::string, Socket> mySockets;

	Skeleton(const TGA::FBX::Skeleton& aSkeleton);
};

