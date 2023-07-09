#pragma once
#include <Vector4.hpp>
#include "ConstantBuffer.h"

struct MaterialBufferData
{
	CommonUtilities::Vector4f AlbedoColor;
	CommonUtilities::Vector2f UVTiling;
	float NormalStrength;
	float Shininess;
};

typedef ConstantBuffer<MaterialBufferData> MaterialBuffer;