#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderMesh.h"

GfxCmd_RenderMesh::GfxCmd_RenderMesh(const MeshComponent& aMesh, bool aIsDeferred) : GraphicsCommand(aIsDeferred ? RenderStage::Deferred : RenderStage::Forward), myMeshElements(aMesh.GetElements()), myTransformMatrix(aMesh.GetTransform()), 
myWorldPosition(aMesh.GetWorldPosition()), myColor(aMesh.GetColor()), myHasBones(false), myBoneTransforms(), myID(aMesh.GetParentID())
#ifdef _DEBUG
, myMeshName(aMesh.GetName())
#endif // _DEBUG
{
}

GfxCmd_RenderMesh::GfxCmd_RenderMesh(const AnimatedMeshComponent& aMesh, bool aIsDeferred) : GraphicsCommand(aIsDeferred ? RenderStage::Deferred : RenderStage::Forward), myMeshElements(aMesh.GetElements()), myTransformMatrix(aMesh.GetTransform()), 
myWorldPosition(aMesh.GetWorldPosition()), myColor(aMesh.GetColor()), myHasBones(true), myBoneTransforms(aMesh.GetBoneTransforms()), myID(aMesh.GetParentID())
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

const Crimson::Vector4f& GfxCmd_RenderMesh::GetWorldPosition() const
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
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, GetObjectBufferSlot(), buffer);
}

void GfxCmd_RenderMesh::SetMaterialResource(const Material& aMaterial)
{
	RHI::SetVertexShader(aMaterial.myVertexShader);
	if (myStage == RenderStage::Forward)
	{
		RHI::SetPixelShader(aMaterial.myPixelShader);
	}
	
	RHI::UpdateConstantBufferData(aMaterial.myBuffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, GetMaterialBufferSlot(), aMaterial.myBuffer);

	if (aMaterial.myAlbedoTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Albedo, aMaterial.myAlbedoTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Albedo, GetMissingTexture());
	}

	if (aMaterial.myNormalTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Normal, aMaterial.myNormalTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Normal, GetDefaultNormalTexture());
	}

	if (aMaterial.myMaterialTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Material, aMaterial.myMaterialTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Material, GetDefaultMaterialTexture());
	}

	if (aMaterial.myFXTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_FX, aMaterial.myFXTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_FX, GetDefaultFXTexture());
	}

	for (auto& texture : aMaterial.myTextures)
	{
		RHI::SetTextureResource(texture.stage, texture.slot, texture.texture);
	}
}
