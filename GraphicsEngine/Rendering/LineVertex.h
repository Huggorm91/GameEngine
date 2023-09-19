#pragma once
#include "Color.h"
#include <Vector4.hpp>
#include <wrl.h>
using namespace Microsoft::WRL;

struct LineVertex
{
	CommonUtilities::Vector4f myPosition;
	CommonUtilities::Vector4f myColor;
	float myThickness;

	LineVertex();
	LineVertex(float aX, float aY, float aZ, float aR, float aG, float aB);
	LineVertex(CommonUtilities::Vector3f aPosition, CommonUtilities::Vector4f aColor = ColorManager::GetColor("White"));

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDefinition;
	static ComPtr<ID3D11InputLayout> InputLayout;
};

