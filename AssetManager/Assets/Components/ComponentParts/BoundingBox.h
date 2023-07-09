#pragma once
#include <Vector3.hpp>
#include <TgaFbxStructs.h>

struct BoundingBox
{
	CommonUtilities::Vector3f myMin;
	CommonUtilities::Vector3f myMax;
	bool myIsValid;

	BoundingBox();
	BoundingBox(const TGA::FBX::Box& aBounds);
};