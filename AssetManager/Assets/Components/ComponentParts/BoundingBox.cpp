#include "AssetManager.pch.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox(): myMin(), myMax(), myIsValid()
{
}

BoundingBox::BoundingBox(const TGA::FBX::Box& aBounds): myMin(aBounds.Min[0], aBounds.Min[1], aBounds.Min[2]), myMax(aBounds.Max[0], aBounds.Max[1], aBounds.Max[2]), myIsValid(aBounds.IsValid)
{
}