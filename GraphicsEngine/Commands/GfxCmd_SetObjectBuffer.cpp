#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetObjectBuffer.h"

GfxCmd_SetObjectBuffer::GfxCmd_SetObjectBuffer(const MeshComponent& aMesh): myTransformMatrix(aMesh.GetTransform()), myColor(aMesh.GetColor()), myHasBones(false), myBoneTransforms()
{
}

GfxCmd_SetObjectBuffer::GfxCmd_SetObjectBuffer(const AnimatedMeshComponent& aMesh) : myTransformMatrix(aMesh.GetTransform()), myColor(aMesh.GetColor()), myHasBones(true), myBoneTransforms(aMesh.GetBoneTransforms())
{
}

void GfxCmd_SetObjectBuffer::Execute()
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
