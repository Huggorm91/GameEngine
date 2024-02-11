#pragma once
#include "Color.h"
#include "Math/Vector4.hpp"
#include <wrl.h>
using namespace Microsoft::WRL;

struct ParticleVertex
{
	Crimson::Vector4f myPosition;
	Crimson::Vector4f myColor;
	Crimson::Vector3f myVelocity;
	Crimson::Vector3f myScale;
	float myLifeTime;

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDefinition;
	static ComPtr<ID3D11InputLayout> InputLayout;
};
