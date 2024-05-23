#pragma once
#include "CrimsonUtilities/Math/Vector4.hpp"
#include "ThirdParty/FBXImporter/include/TgaFbxStructs.h"
#include <wrl.h>
using namespace Microsoft::WRL;

// We'll be writing this a lot so easier to just typedef it here.
typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementList;

struct Vertex
{
	Vertex();
	Vertex(const TGA::FBX::Vertex& aVertex);
	Vertex(const Crimson::Vector3f& aPosition, const Crimson::Vector2f& aUV, const Crimson::Vector3f& aNormal, const Crimson::Vector3f& aTangent);

	Crimson::Vector2f myUVs[4];
	Crimson::Vector4f myColors[4];

	Crimson::Vector4f myPosition;

	Crimson::Vector4ui myBoneIDs;
	Crimson::Vector4f myBoneWeights;

	Crimson::Vector3f myNormal;
	Crimson::Vector3f myTangent;

	static const InputElementList InputLayoutDefinition;
	static ComPtr<ID3D11InputLayout> InputLayout;
};

