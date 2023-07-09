#include "GraphicsEngine.pch.h"
#include "Color.h"

CommonUtilities::Vector4f GetColor(eColor aColor)
{
	switch (aColor)
	{
	case eColor::Red:
	{
		return { 1.f, 0.f, 0.f, 1.f };
	}
	case eColor::Blue:
	{
		return { 0.f, 0.f, 1.f, 1.f };
	}
	case eColor::Green:
	{
		return { 0.f, 1.f, 0.f, 1.f };
	}
	case eColor::Cyan:
	{
		return { 0.f, 1.f, 1.f, 1.f };
	}
	case eColor::Purple:
	{
		return { 1.f, 0.f, 1.f, 1.f };
	}
	case eColor::White:
	{
		return { 1.f, 1.f, 1.f, 1.f };
	}
	case eColor::Black:
	{
		return { 0.f, 0.f, 0.f, 1.f };
	}
	case eColor::Yellow:
	{
		return { 1.f, 1.f, 0.f, 1.f };
	}
	case eColor::Gray:
	{
		return { .2f, .2f, .2f, 1.f };
	}
	case eColor::Brown:
	{
		return { .4f, .2f, 0.f, 1.f };
	}
	case eColor::Orange:
	{
		return { 1.f, .4f, 0.f, 1.f };
	}
	default:
		return {};
	}
}

CommonUtilities::Vector4f GetColor(const CommonUtilities::Vector3f& aHexValue)
{
    return CommonUtilities::Vector4f(aHexValue/255.f, 1.f);
}
