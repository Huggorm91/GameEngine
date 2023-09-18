#pragma once
#include <Vector4.hpp>
#include "ConstantBuffer.h"

struct MaterialBufferData
{
	CommonUtilities::Vector4f AlbedoColor{};
	CommonUtilities::Vector3f EmissionColor{};
	float EmissionIntensity{};
	CommonUtilities::Vector2f UVTiling{};
	float NormalStrength{};
	float Shininess{};
	float Metalness{};

	CommonUtilities::Vector3f padding;
};

typedef ConstantBuffer<MaterialBufferData> MaterialBuffer;