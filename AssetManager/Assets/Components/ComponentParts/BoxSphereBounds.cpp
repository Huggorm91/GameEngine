#include "AssetManager.pch.h"
#include "BoxSphereBounds.h"

BoxSphereBounds::BoxSphereBounds(): myBoxExtents(), myCenter(), myRadius(), myIsValid()
{
}

BoxSphereBounds::BoxSphereBounds(const TGA::FBX::BoxSphereBounds& aBounds): myBoxExtents(aBounds.BoxExtents[0], aBounds.BoxExtents[1], aBounds.BoxExtents[2]), myCenter(aBounds.Center[0], aBounds.Center[1], aBounds.Center[2]), myRadius(aBounds.Radius), myIsValid()
{
	if (myRadius != 0.f || myBoxExtents != CommonUtilities::Vector3f::Null)
	{
		myIsValid = true;
	}
}

BoxSphereBounds::BoxSphereBounds(const TGA::FBX::Box& aBounds) : 
	myBoxExtents((CommonUtilities::Vector3f{aBounds.Max[0], aBounds.Max[1], aBounds.Max[2]} - CommonUtilities::Vector3f{ aBounds.Min[0], aBounds.Min[1], aBounds.Min[2] }) * .5f), 
	myCenter((CommonUtilities::Vector3f{ aBounds.Min[0], aBounds.Min[1], aBounds.Min[2] } + CommonUtilities::Vector3f{aBounds.Max[0], aBounds.Max[1], aBounds.Max[2]}) * .5f), 
	myRadius(myBoxExtents.Length()), myIsValid(aBounds.IsValid)
{
	// This represents the initialization above
	/*CommonUtilities::Vector3f min {aBounds.Min[0], aBounds.Min[1], aBounds.Min[2]};
	CommonUtilities::Vector3f max {aBounds.Max[0], aBounds.Max[1], aBounds.Max[2]};
	myCenter = (min + max) * 0.5f;
	myBoxExtents = (max - min) * 0.5f;
	myRadius = myBoxExtents.Length();*/
}

CommonUtilities::Vector3f BoxSphereBounds::GetMin() const
{
	return myCenter - myBoxExtents;
}

CommonUtilities::Vector3f BoxSphereBounds::GetMax() const
{
	return myCenter + myBoxExtents;
}

CommonUtilities::Vector3f BoxSphereBounds::GetBoxSize() const
{
	return myBoxExtents * 2.f;
}

const CommonUtilities::Vector3f& BoxSphereBounds::GetBoxExtents() const
{
	return myBoxExtents;
}

const CommonUtilities::Vector3f& BoxSphereBounds::GetCenter() const
{
	return myCenter;
}

float BoxSphereBounds::GetRadius() const
{
	return myRadius;
}

bool BoxSphereBounds::IsValid() const
{
	return myIsValid;
}

