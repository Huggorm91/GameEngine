#include "AssetManager.pch.h"
#include "Socket.h"

Socket::Socket(TGA::FBX::Skeleton::Socket aSocket): 
	myParentBoneIdx(aSocket.ParentBoneIdx), 
	myNamespaceName(aSocket.NamespaceName), 
	myName(aSocket.Name), 
	myRestTransform(ConvertMatrix(aSocket.RestTransform))
{
}

Socket::Socket(std::istream& aStream): myParentBoneIdx()
{
	myRestTransform.Deserialize(aStream);
	std::getline(aStream, myNamespaceName, '\0');
	std::getline(aStream, myName, '\0');
	aStream.read(reinterpret_cast<char*>(&myParentBoneIdx), sizeof(myParentBoneIdx));
}

void Socket::Serialize(std::ostream& aStream) const
{
	myRestTransform.Serialize(aStream);
	aStream.write(myNamespaceName.c_str(), myNamespaceName.size() + 1);
	aStream.write(myName.c_str(), myName.size() + 1);
	aStream.write(reinterpret_cast<const char*>(&myParentBoneIdx), sizeof(myParentBoneIdx));
}
