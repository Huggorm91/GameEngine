#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderMesh.h"

GfxCmd_RenderMesh::GfxCmd_RenderMesh(const MeshComponent& aMesh): myMeshElements(aMesh.GetElements())
#ifdef _DEBUG
, myMeshName(aMesh.GetName())
#endif // _DEBUG
{
}

void GfxCmd_RenderMesh::Execute()
{
	for (auto& element : myMeshElements)
	{
		SetMaterialResource(element.myMaterial);
		RHI::ConfigureInputAssembler(element.myData->myPrimitiveTopology, element.myData->myVertexBuffer, element.myData->myIndexBuffer, element.myData->myStride, Vertex::InputLayout);
		RHI::DrawIndexed(element.myData->myIndexCount);
	}
}

void GfxCmd_RenderMesh::SetMaterialResource(const Material& aMaterial)
{
	RHI::SetVertexShader(aMaterial.GetVertexShader());
	RHI::SetPixelShader(aMaterial.GetPixelShader());
	auto& buffer = GetMaterialBuffer();
	buffer.Data.AlbedoColor = aMaterial.GetColor();
	buffer.Data.UVTiling = aMaterial.GetUVTiling();
	buffer.Data.NormalStrength = aMaterial.GetNormalStrength();
	buffer.Data.Shininess = aMaterial.GetShininess();
	buffer.Data.Metalness = aMaterial.GetMetalness();
	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 3, buffer);

	if (aMaterial.GetAlbedoTexture())
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, aMaterial.GetAlbedoTexture());
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, GetMissingTexture());
	}

	if (aMaterial.GetNormalTexture())
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, aMaterial.GetNormalTexture());
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, GetDefaultNormalTexture());
	}

	if (aMaterial.GetMaterialTexture())
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 2, aMaterial.GetMaterialTexture());
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 2, GetDefaultMaterialTexture());
	}
}
