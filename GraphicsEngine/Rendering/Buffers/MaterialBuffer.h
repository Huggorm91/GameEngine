#pragma once
#include <Vector4.hpp>
#include "ConstantBuffer.h"

struct MaterialBufferData
{
	CommonUtilities::Vector4f AlbedoColor{};
	CommonUtilities::Vector4f EmissionColor{};
	float EmissionIntensity{};
	CommonUtilities::Vector2f UVTiling{};
	float NormalStrength{};
	float Shininess{};
	float Metalness{};

	CommonUtilities::Vector2f padding;
};

typedef ConstantBuffer<MaterialBufferData> MaterialBuffer;