#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderMesh.h"

GfxCmd_RenderMesh::GfxCmd_RenderMesh(const MeshComponent& aMesh): myMeshElements(aMesh.GetElements()), myTransformMatrix(aMesh.GetTransform()), myWorldPosition(aMesh.GetWorldPosition()), myColor(aMesh.GetColor()), myHasBones(false), myBoneTransforms()
#ifdef _DEBUG
, myMeshName(aMesh.GetName())
#endif // _DEBUG
{
}

GfxCmd_RenderMesh::GfxCmd_RenderMesh(const AnimatedMeshComponent& aMesh) : myMeshElements(aMesh.GetElements()), myTransformMatrix(aMesh.GetTransform()), myWorldPosition(aMesh.GetWorldPosition()), myColor(aMesh.GetColor()), myHasBones(true), myBoneTransforms(aMesh.GetBoneTransforms())
#ifdef _DEBUG
, myMeshName(aMesh.GetName())
#endif // _DEBUG
{
}

void GfxCmd_RenderMesh::Execute()
{
	SetObjectBuffer();
	for (auto& element : myMeshElements)
	{
		SetMaterialResource(element.myMaterial);
		RHI::ConfigureInputAssembler(element.myData->myPrimitiveTopology, element.myData->myVertexBuffer, element.myData->myIndexBuffer, element.myData->myStride, Vertex::InputLayout);
		RHI::DrawIndexed(element.myData->myIndexCount);
	}
}

const CommonUtilities::Vector3f& GfxCmd_RenderMesh::GetWorldPosition() const
{
	return myWorldPosition;
}

void GfxCmd_RenderMesh::SetObjectBuffer()
{
	ObjectBuffer& buffer = GetObjectBuffer();
	buffer.Data.Transform = myTransformMatrix;
	buffer.Data.Color = myColor;
	buffer.Data.HasBones = myHasBones;
	if (myHasBones)
	{
		std::copy(myBoneTransforms.begin(), myBoneTransforms.end(), buffer.Data.BoneTransforms);
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 1, buffer);
}

void GfxCmd_RenderMesh::SetMaterialResource(const Material& aMaterial)
{
	RHI::SetVertexShader(aMaterial.myVertexShader);
	RHI::SetPixelShader(aMaterial.myPixelShader);
	auto& buffer = GetMaterialBuffer();
	buffer.Data.AlbedoColor = aMaterial.myAlbedoColor;
	buffer.Data.UVTiling = aMaterial.myUVTiling;
	buffer.Data.NormalStrength = aMaterial.myNormalStrength;
	buffer.Data.Shininess = aMaterial.myShininess;
	buffer.Data.Metalness = aMaterial.myMetalness;
	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 3, buffer);

	if (aMaterial.myAlbedoTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, aMaterial.myAlbedoTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, GetMissingTexture());
	}

	if (aMaterial.myNormalTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, aMaterial.myNormalTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, GetDefaultNormalTexture());
	}

	if (aMaterial.myMaterialTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 2, aMaterial.myMaterialTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 2, GetDefaultMaterialTexture());
	}

	for (auto& texture : aMaterial.myTextures)
	{
		RHI::SetTextureResource(texture.stage, texture.slot, texture.texture);
	}
}
