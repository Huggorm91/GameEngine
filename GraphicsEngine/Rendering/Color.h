#pragma once
#include <Vector4.hpp>

enum class eColor
{
	Black,
	White,
	Red,
	Blue,
	Green,
	Cyan,
	Purple,
	Yellow,
	Orange,
	Brown,
	Gray
};

CommonUtilities::Vector4f GetColor(eColor aColor);
CommonUtilities::Vector4f GetColor(const CommonUtilities::Vector3f& aHexValue);