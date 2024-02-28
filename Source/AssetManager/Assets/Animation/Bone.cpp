#include "AssetManager.pch.h"
#include "Bone.h"

Bone::Bone(const TGA::FBX::Skeleton::Bone& aBone) : parent(aBone.ParentIdx), namespaceName(aBone.NamespaceName), name(aBone.Name), children(aBone.Children), bindPoseInverse(ConvertTransposedMatrix(aBone.BindPoseInverse))
{
}
