#pragma once
#include "Color.h"
#include "Math/Vector4.hpp"
#include <wrl.h>
using namespace Microsoft::WRL;

struct ParticleVertex
{
	Crimson::Vector4f myPosition;
	Crimson::Vector4f myColor;
	float myThickness;

	ParticleVertex();
	ParticleVertex(float aX, float aY, float aZ, float aR, float aG, float aB);
	ParticleVertex(Crimson::Vector3f aPosition, Crimson::Vector4f aColor = ColorManager::GetColor("White"));

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDefinition;
	static ComPtr<ID3D11InputLayout> InputLayout;
};
