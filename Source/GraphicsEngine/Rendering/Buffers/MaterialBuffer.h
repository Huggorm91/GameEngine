#pragma once
#include "CrimsonUtilities/Math/Vector4.hpp"
#include "ConstantBuffer.h"

struct MaterialBufferData
{
	Crimson::Vector4f AlbedoColor{};
	Crimson::Vector4f EmissionColor{};
	float EmissionIntensity{};
	Crimson::Vector2f UVTiling{};
	float NormalStrength{};
	float Shininess{};
	float Metalness{};

	Crimson::Vector2f padding;
};

typedef ConstantBuffer<MaterialBufferData> MaterialBuffer;