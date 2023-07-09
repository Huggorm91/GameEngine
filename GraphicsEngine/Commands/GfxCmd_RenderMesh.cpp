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
	//RHI::UpdateConstantBufferData(aMaterial.GetBuffer());
	//RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 3, aMaterial.GetBuffer());

	if (aMaterial.GetTexture())
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, aMaterial.GetTexture());
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
}
