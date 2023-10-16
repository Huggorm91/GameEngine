#pragma once
#include <d3d11.h>
#include "GraphicsEngine/InterOp/RHI.h"
#include "GraphicsEngine/Rendering/Vertex.h"
#include "GraphicsEngine/Rendering/Material.h"
#include "BoxSphereBounds.h"

struct MeshData
{
	MeshData();
	MeshData(ComPtr<ID3D11Buffer> aVertexBuffer, ComPtr<ID3D11Buffer> anIndexBuffer, UINT aVertexCount, UINT anIndexCount, UINT aPrimitiveTopology, UINT aStride);
	MeshData(const TGA::FBX::Mesh::Element& anElement);
	MeshData(const MeshData& anElement);

	BoxSphereBounds myBoxSphereBounds;
	std::string myMeshName;

	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;
	UINT myVertexCount;
	UINT myIndexCount;
	UINT myPrimitiveTopology;
	UINT myStride;
};

struct MeshElement
{
	MeshElement();
	MeshElement(const MeshData& someData);
	MeshElement(const MeshElement& anElement);

	const MeshData* myData;
	Material myMaterial;
};