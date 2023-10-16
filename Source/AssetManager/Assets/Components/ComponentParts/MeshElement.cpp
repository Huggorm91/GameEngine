#include "AssetManager.pch.h"
#include "MeshElement.h"
#include "GraphicsEngine/GraphicsEngine.h"

MeshElement::MeshElement(): myData(nullptr), myMaterial(GraphicsEngine::Get().GetDefaultMaterial())
{
}

MeshElement::MeshElement(const MeshData& someData): myData(&someData), myMaterial(GraphicsEngine::Get().GetDefaultMaterial())
{
}

MeshElement::MeshElement(const MeshElement& anElement) : myData(anElement.myData), myMaterial(anElement.myMaterial)
{
}

MeshData::MeshData() : myBoxSphereBounds(), myMeshName(), myVertexBuffer(), myIndexBuffer(), myVertexCount(), myIndexCount(), myPrimitiveTopology(), myStride()
{
}

MeshData::MeshData(ComPtr<ID3D11Buffer> aVertexBuffer, ComPtr<ID3D11Buffer> anIndexBuffer, UINT aVertexCount, UINT anIndexCount, UINT aPrimitiveTopology, UINT aStride) :
	myVertexBuffer(aVertexBuffer), myIndexBuffer(anIndexBuffer), myVertexCount(aVertexCount), myIndexCount(anIndexCount), myPrimitiveTopology(aPrimitiveTopology), myStride(aStride), myBoxSphereBounds(), myMeshName()
{
}

MeshData::MeshData(const TGA::FBX::Mesh::Element& anElement) : myBoxSphereBounds(anElement.BoxSphereBounds), myVertexBuffer(), myIndexBuffer(), myVertexCount(static_cast<UINT>(anElement.Vertices.size())),
myIndexCount(static_cast<UINT>(anElement.Indices.size())), myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), myStride(sizeof(Vertex)), myMeshName(anElement.MeshName)
{
	if (anElement.BoxBounds.IsValid && anElement.BoxSphereBounds.Radius == 0.f)
	{
		myBoxSphereBounds = anElement.BoxBounds;
	}
}

MeshData::MeshData(const MeshData& anElement) : myVertexBuffer(anElement.myVertexBuffer), myIndexBuffer(anElement.myIndexBuffer), myVertexCount(anElement.myVertexCount),
myIndexCount(anElement.myIndexCount), myPrimitiveTopology(anElement.myPrimitiveTopology), myStride(anElement.myStride), myBoxSphereBounds(anElement.myBoxSphereBounds), myMeshName(anElement.myMeshName)
{
}
