#include "AssetManager.pch.h"
#include "Bone.h"

Bone::Bone(const TGA::FBX::Skeleton::Bone& aBone) : 
	bindPoseInverse(ConvertTransposedMatrix(aBone.BindPoseInverse)),
	bindPose(bindPoseInverse.GetInverse()),
	children(aBone.Children), 
	namespaceName(aBone.NamespaceName), 
	name(aBone.Name), 
	parent(aBone.ParentIdx) 
{}