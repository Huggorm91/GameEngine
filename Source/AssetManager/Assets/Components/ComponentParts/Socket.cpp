#include "AssetManager.pch.h"
#include "Socket.h"

Socket::Socket(TGA::FBX::Skeleton::Socket aSocket): myParentBoneIdx(aSocket.ParentBoneIdx), myNamespaceName(aSocket.NamespaceName), myName(aSocket.Name), myRestTransform(ConvertMatrix(aSocket.RestTransform))
{
}
