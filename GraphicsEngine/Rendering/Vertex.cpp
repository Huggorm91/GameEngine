#include "GraphicsEngine.pch.h"
#include "Vertex.h"

const InputElementList Vertex::InputLayoutDefinition =
{
	{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ComPtr<ID3D11InputLayout> Vertex::InputLayout;

Vertex::Vertex() : myPosition(), myColors(), myBoneIDs(), myBoneWeights(), myUVs(), myNormal(), myTangent()
{
}

Vertex::Vertex(const TGA::FBX::Vertex& aVertex) : myUVs{ {aVertex.UVs[0][0], aVertex.UVs[0][1]}, {aVertex.UVs[1][0], aVertex.UVs[1][1]}, {aVertex.UVs[2][0], aVertex.UVs[2][1]}, {aVertex.UVs[3][0], aVertex.UVs[3][1]} },
myColors{ {aVertex.VertexColors[0][0], aVertex.VertexColors[0][1], aVertex.VertexColors[0][2], aVertex.VertexColors[0][3]}, {aVertex.VertexColors[1][0], aVertex.VertexColors[1][1], aVertex.VertexColors[1][2], aVertex.VertexColors[1][3]}, {aVertex.VertexColors[2][0], aVertex.VertexColors[2][1], aVertex.VertexColors[2][2], aVertex.VertexColors[2][3]}, {aVertex.VertexColors[3][0], aVertex.VertexColors[3][1], aVertex.VertexColors[3][2], aVertex.VertexColors[3][3]} },
myPosition(aVertex.Position[0], aVertex.Position[1], aVertex.Position[2], aVertex.Position[3]),
myBoneIDs(aVertex.BoneIDs[0], aVertex.BoneIDs[1], aVertex.BoneIDs[2], aVertex.BoneIDs[3]),
myBoneWeights(aVertex.BoneWeights[0], aVertex.BoneWeights[1], aVertex.BoneWeights[2], aVertex.BoneWeights[3]),
myNormal(aVertex.Normal[0], aVertex.Normal[1], aVertex.Normal[2]),
myTangent(aVertex.Tangent[0], aVertex.Tangent[1], aVertex.Tangent[2])
{
}

Vertex::Vertex(const Crimson::Vector3f& aPosition, const Crimson::Vector2f& aUV, const Crimson::Vector3f& aNormal, const Crimson::Vector3f& aTangent) :
	myPosition(aPosition, 1.f), myColors{ {0.f, 0.f, 0.f, 1.f} }, myBoneIDs(), myBoneWeights(), myUVs{ aUV }, myNormal(aNormal), myTangent(aTangent)
{
}
