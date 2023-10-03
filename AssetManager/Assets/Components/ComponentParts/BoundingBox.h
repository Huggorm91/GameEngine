#pragma once
#include "Math/Vector3.hpp"
#include <TgaFbxStructs.h>

struct BoundingBox
{
	Crimson::Vector3f myMin;
	Crimson::Vector3f myMax;
	bool myIsValid;

	BoundingBox();
	BoundingBox(const TGA::FBX::Box& aBounds);
};