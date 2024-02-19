#include "AssetManager.pch.h"
#include "Bone.h"

Bone::Bone(const TGA::FBX::Skeleton::Bone& aBone) : myParent(aBone.ParentIdx), myNamespaceName(aBone.NamespaceName), myName(aBone.Name), myChildren(aBone.Children), myBindPoseInverse(ConvertTransposedMatrix(aBone.BindPoseInverse))
{
}
