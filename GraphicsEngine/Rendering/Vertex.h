#pragma once
#include <TgaFbxStructs.h>
#include <Vector4.hpp>
#include <wrl.h>
using namespace Microsoft::WRL;

// We'll be writing this a lot so easier to just typedef it here.
typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementList;

struct Vertex
{
	Vertex();
	Vertex(const TGA::FBX::Vertex& aVertex);
	Vertex(const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector4f& aColor);
	Vertex(const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector4f& aColor, const CommonUtilities::Vector2f& aUV);
	Vertex(const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector2f& aUV, const CommonUtilities::Vector3f& aNormal, const CommonUtilities::Vector3f& aTangent);

	CommonUtilities::Vector2f myUVs[4];
	CommonUtilities::Vector4f myColors[4];

	CommonUtilities::Vector4f myPosition;

	CommonUtilities::Vector4ui myBoneIDs;
	CommonUtilities::Vector4f myBoneWeights;

	CommonUtilities::Vector3f myNormal;
	CommonUtilities::Vector3f myTangent;

	static const InputElementList InputLayoutDefinition;
	static ComPtr<ID3D11InputLayout> InputLayout;
};

